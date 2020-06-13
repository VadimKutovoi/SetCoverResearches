#include <random>
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>

#include "../graph/generators.h"
#include "../vertex_cover/vertex_cover.h"

#define GRAPH_SIZE 30
#define GRAPH_VERTEX_CONN_PROBABILITY 0.1
#define GRAPH_VERTEX_MAX_WEIGHT_RATIO 1.5

bool check_solution(const graph* g, gcont cover) {
    graph tmp(*g);

    auto num_vertex = tmp.getVertNum();
    for (const vmark &vertex : cover) {
        auto adj_list = tmp.getAdjList(vertex);
        for (const vmark &adj_vertex : adj_list) {
            tmp.removeEdge(vertex, adj_vertex);
        }
    }

    if (tmp.getNumEdges() != 0) {
        std::cout << "INIT NUM EDGES IS " << g->getNumEdges() << std::endl;
        std::cout << "AFTER REMOVING " << tmp.getNumEdges() << std::endl;
        return false;
    }

    return true;
}

class InputParser{
 public:
    InputParser(int &argc, char **argv) {
        for (int i=1; i < argc; ++i)
            this->tokens.push_back(std::string(argv[i]));
    }

    const std::string& getCmdOption(const std::string &option) const {
        std::vector<std::string>::const_iterator itr;
        itr =  std::find(this->tokens.begin(), this->tokens.end(), option);
        if (itr != this->tokens.end() && ++itr != this->tokens.end()) {
            return *itr;
        }
        static const std::string empty_string("");
        return empty_string;
    }

    bool cmdOptionExists(const std::string &option) const {
        return std::find(this->tokens.begin(), this->tokens.end(), option)
               != this->tokens.end();
    }
 private:
    std::vector <std::string> tokens;
};

int main(int argc, char **argv) {
    int graph_size = GRAPH_SIZE;
    float probability = GRAPH_VERTEX_CONN_PROBABILITY;
    std::string graph_file;
    std::string graph_type;
    std::string output_type;

    InputParser input(argc, argv);

    if (input.cmdOptionExists("-s")) {
        graph_size = std::stoi(input.getCmdOption("-s"));
    }

    if (input.cmdOptionExists("-p")) {
        probability = std::stof(input.getCmdOption("-p"));
    }

    if (input.cmdOptionExists("-m")) {
        graph_file = input.getCmdOption("-m");
    }

    if (input.cmdOptionExists("-misf")) {
        graph_file = input.getCmdOption("-misf");
    }

    if (input.cmdOptionExists("-type")) {
        graph_type = input.getCmdOption("-type");
    }

    if (input.cmdOptionExists("-otype")) {
        output_type = input.getCmdOption("-otype");
    }

    std::cout << "graph_size = " << graph_size << std::endl;
    std::cout << "probability = " << probability << std::endl;

    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution <> dist(0, 100);

    //graph g(generateGridGraph(1000, 500));
    graph g(graph_size);

    if (!input.cmdOptionExists("-m") && !input.cmdOptionExists("-misf")) {
        std::cout << "Generating graph..." << std::endl;

        std::cout << "Generating basic edges..." << std::endl;

        if (graph_type == "hypercube") {
            std::cout << "Generating hypercube..." << std::endl;
            //g = generateHypercubeGraph(graph_size);
        } else if (graph_type == "grid") {
            std::cout << "Generating grid..." << std::endl;
            //g = generateGridGraph(graph_size/2, graph_size/2);
        } else if (graph_type == "random") {
            for (auto i = 0; i < graph_size; i++) {
                std::cout << "Generating edges for vertex " << i << "\r";
                for (auto j = 0; j < graph_size; j++) {
                    if (dist(e2) <= probability * 100 \
                        && i != j && !g.isConnected(i, j)) {
                        g.addEdge(i, j);
                    }
                }
            }
        }

        std::cout << std::endl;
        std::cout << "Generating basic edges... Done!" << std::endl;
        std::cout << "Cheking what all vertices has eges..." << std::endl;

        for (auto i = 0; i < graph_size; i++) {
            if (g.getAdjNum(i) == 0) {
                vmark adj_vertex = static_cast<int>(dist(e2)) \
                                   % graph_size;
                std::cout << "Vertex " << i \
                          << " doesn't have edges, adding edge (" \
                          << i << ";" << adj_vertex << ")" << std::endl;
                g.addEdge(i, adj_vertex);
            }
        }

        std::cout << "Cheking what all vertices has eges... Done!" << std::endl;

        std::cout << "Generating vertex weights..." << std::endl;

        vmark max_weight = 100;
        std::random_device rdmw;
        std::mt19937 emw(rdmw());
        std::uniform_real_distribution <> distmw(1, max_weight);

        for (auto i = 0; i < graph_size; i++) {
            vmark weight = static_cast<int>(distmw(emw));
            g.setVertWeight(i, 1);
        }
        std::cout << "Generating vertex weights... Done!" << std::endl;
    } else if (input.cmdOptionExists("-m")) {
        std::ifstream adj_matrix_file(graph_file);

        for (auto i = 0; i < graph_size; i++) {
            for (auto j = 0; j < graph_size; j++) {
                int is_adj;
                adj_matrix_file >> is_adj;

                if (is_adj && !g.isConnected(i, j)) {
                    g.addEdge(i, j);
                }
            }
        }
    } else if (input.cmdOptionExists("-misf")) {
        std::ifstream adj_matrix_file(graph_file);
        std::string skip;
        vmark num_vert = 0;
        vmark num_edges = 0;

        adj_matrix_file >> skip;
        adj_matrix_file >> skip;

        adj_matrix_file >> num_vert;
        adj_matrix_file >> num_edges;

        for (auto i = 0; i < num_edges; i++) {
            adj_matrix_file >> skip;
            vmark v1 = 0, v2 = 0;

            adj_matrix_file >> v1;
            adj_matrix_file >> v2;

            g.addEdge(v1 - 1, v2 - 1);
        }
    }

    graph_size = g.getVertNum();

    std::cout << "Calculating graph metrics..." << std::endl;

    int num_of_edges = 0;
    for (auto i = 0; i < graph_size; i++) {
        num_of_edges += g.getAdjNum(i);
    }

    std::cout << "Num edges is " \
              << g.getNumEdges() << std::endl;

    auto avg_num_of_edges = num_of_edges/graph_size;
    std::cout << "Average num of edges is " \
              << avg_num_of_edges << std::endl;

    int total_weight = 0;
    for (auto i = 0; i < graph_size; i++) {
        total_weight += g.getVertWeight(i);
    }

    auto avg_weight = total_weight/graph_size;
    std::cout << "Average weight of vertex is " \
              << avg_weight << std::endl;

    std::ofstream adj_list_file;

    if (!input.cmdOptionExists("-nodump")) {
        if (!input.cmdOptionExists("-otype")) {
            adj_list_file.open("adj_list.list");
            for (auto i = 0; i < graph_size; i++) {
                gcont adj_list = g.getAdjList(i);
                adj_list_file << i << " ";
                for (auto it = adj_list.begin(); it != adj_list.end(); it++) {
                    adj_list_file << *it << " ";
                }
                adj_list_file << std::endl;
            }
        } else if (output_type == "netrep") {
            adj_list_file.open("graph.graph");
            adj_list_file << g.getVertNum() << " " << g.getNumEdges() << " " << 0 << std::endl;
            for (auto i = 0; i < graph_size; i++) {
                gcont adj_list = g.getAdjList(i);
                for (auto it = adj_list.begin(); it != adj_list.end(); it++) {
                    adj_list_file << *it + 1 << " ";
                }
                adj_list_file << std::endl;
            }
        } else if (output_type == "mis") {
            adj_list_file.open("graph.mis");
            adj_list_file << "p" << " " << "edge " << g.getVertNum() << " " << g.getNumEdges() << std::endl;
            for (auto i = 0; i < graph_size; i++) {
                gcont adj_list = g.getAdjList(i);
                for (auto it = adj_list.begin(); it != adj_list.end(); it++) {
                    if (i < *it)
                        adj_list_file << "e " << i + 1 << " " << *it + 1 << std::endl;
                }
            }
        }
    }

    std::cout << "Calculating graph metrics... Done!" << std::endl;

    std::cout << "Generating graph... Done!" << std::endl;

    vmark solution_weight = 0;
    vmark cover_size = 0;
    gcont vertex_cover;
    float reduction_ratio = 0;

    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;

    // TWO APPROXIMATE

    solution_weight = 0;
    cover_size = 0;
    reduction_ratio = 0;

    std::cout << "TwoApproximate..." << std::endl;

    begin = std::chrono::steady_clock::now();
    vertex_cover = vertexCoverTwoApproximate(&g, 5);
    end = std::chrono::steady_clock::now();

    std::cout << "TwoApproximate... Done! " << "Time = " \
              << std::chrono::duration_cast<std::chrono::\
              microseconds>(end - begin).count() << "[µs]" << std::endl;

    std::ofstream cover_list_twoapprox_file;
    cover_list_twoapprox_file.open("stats/cover_list_twoapprox.txt");
    for (const vmark &it : vertex_cover) {
        cover_list_twoapprox_file << it << " ";
        solution_weight += g.getVertWeight(it);
    }
    cover_list_twoapprox_file.close();
    std::cout << "Two approximate solution weight is " \
              << solution_weight << std::endl;


    cover_size = vertex_cover.size();
    std::cout << "TwoApproximate size : " \
              << cover_size << std::endl;

    reduction_ratio = \
                    static_cast<float>(cover_size)/graph_size;

    std::cout << "TwoApproximate rrate : " \
              << reduction_ratio << std::endl;

    // TWO APPROXIMATE END

    // PRIMAL-DUAL

#ifdef USE_PD
    solution_weight = 0;
    cover_size = 0;
    reduction_ratio = 0;

    begin = std::chrono::steady_clock::now();
    vertex_cover = vertexCoverPrimalDual(&g);
    end = std::chrono::steady_clock::now();

    std::cout << "PrimalDual... Done! " << "Time = " \
              << std::chrono::duration_cast<std::chrono::\
              microseconds>(end - begin).count() << "[µs]" << std::endl;

    std::ofstream cover_list_primal_dual_file;
    cover_list_primal_dual_file.open("cover_list_primal_dual.txt");
    for (const vmark &it : vertex_cover) {
        cover_list_primal_dual_file << it << " ";
        solution_weight += g.getVertWeight(it);
    }
    cover_list_primal_dual_file.close();

    std::cout << "PrimalDual solution weight is " \
              << solution_weight << std::endl;

    cover_size = vertex_cover.size();
    std::cout << "PrimalDual size : " \
              << cover_size << std::endl;

    reduction_ratio = \
                    static_cast<float>(cover_size)/graph_size;

    std::cout << "PrimalDual rrate : " \
              << reduction_ratio << std::endl;
#endif


    // PRIMAL-DUAL END

    // HeuristicLR

    //solution_weight = 0;
    //cover_size = 0;
    //reduction_ratio = 0;
//
    //begin = std::chrono::steady_clock::now();
    //vertex_cover = vertexCoverHeuristicLR(&g);
    //end = std::chrono::steady_clock::now();
//
    //if (check_solution(&g, vertex_cover)) {
    //    std::cout << "HeuristicLR... Done! " << "Time = " \
    //              << std::chrono::duration_cast<std::chrono::\
    //              microseconds>(end - begin).count() << "[µs]" << std::endl;
//
    //    std::ofstream cover_list_lr_file;
    //    cover_list_lr_file.open("stats/cover_list_lr_file.txt");
    //    solution_weight = 0;
    //    for (const vmark &it : vertex_cover) {
    //        cover_list_lr_file << it << " ";
    //        solution_weight += g.getVertWeight(it);
    //    }
    //    cover_list_lr_file.close();
//
    //    std::cout << "HeuristicLR solution weight is " \
    //              << solution_weight << std::endl;
//
    //    cover_size = vertex_cover.size();
    //    std::cout << "HeuristicLR size : " \
    //              << cover_size << std::endl;
//
    //    reduction_ratio = \
    //                    static_cast<float>(cover_size)/graph_size;
//
    //    std::cout << "HeuristicLR rrate : " \
    //              << reduction_ratio << std::endl;
    //} else {
    //    std::cout << "HeuristicLR failed!" << std::endl;
    //}
    // HeuristicLR END

//    // HeuristicLL
//
//    solution_weight = 0;
//    cover_size = 0;
//    reduction_ratio = 0;
//
//    begin = std::chrono::steady_clock::now();
//    vertex_cover = vertexCoverHeuristicLL2(&g);
//    end = std::chrono::steady_clock::now();
//
//    if (check_solution(&g, vertex_cover)) {
//        std::cout << "HeuristicLL... Done! " << "Time = " \
//                  << std::chrono::duration_cast<std::chrono::\
//                  microseconds>(end - begin).count() << "[µs]" << std::endl;
//
//        std::ofstream cover_list_ll_file;
//        cover_list_ll_file.open("stats/cover_list_ll_file.txt");
//        solution_weight = 0;
//        for (const vmark &it : vertex_cover) {
//            cover_list_ll_file << it << " ";
//            solution_weight += g.getVertWeight(it);
//        }
//        cover_list_ll_file.close();
//
//        std::cout << "HeuristicLL solution weight is " \
//                  << solution_weight << std::endl;
//
//        cover_size = vertex_cover.size();
//        std::cout << "HeuristicLL size : " \
//                  << cover_size << std::endl;
//
//        reduction_ratio = \
//                        static_cast<float>(cover_size)/graph_size;
//
//        std::cout << "HeuristicLL rrate : " \
//                  << reduction_ratio << std::endl;
//    } else {
//        std::cout << "HeuristicLL failed!" << std::endl;
//    }

    // HeuristicLL END

    
    // HeuristicLD

    solution_weight = 0;
    cover_size = 0;
    reduction_ratio = 0;

    begin = std::chrono::steady_clock::now();
    vertex_cover = vertexCoverHeuristicLD(&g);
    end = std::chrono::steady_clock::now();

    if (check_solution(&g, vertex_cover)) {
        std::cout << "HeuristicLD... Done! " << "Time = " \
                  << std::chrono::duration_cast<std::chrono::\
                  microseconds>(end - begin).count() << "[µs]" << std::endl;

        std::ofstream cover_list_ld_file;
        cover_list_ld_file.open("stats/cover_list_ld_file.txt");
        solution_weight = 0;
        for (const vmark &it : vertex_cover) {
            cover_list_ld_file << it << " ";
            solution_weight += g.getVertWeight(it);
        }
        cover_list_ld_file.close();

        std::cout << "HeuristicLD solution weight is " \
                  << solution_weight << std::endl;

        cover_size = vertex_cover.size();
        std::cout << "HeuristicLD size : " \
                  << cover_size << std::endl;

        reduction_ratio = \
                        static_cast<float>(cover_size)/graph_size;

        std::cout << "HeuristicLD rrate : " \
                  << reduction_ratio << std::endl;
    } else {
        std::cout << "HeuristicLD failed!" << std::endl;
    }

    // HeuristicLD END


    // HeuristicSLL

    solution_weight = 0;
    cover_size = 0;
    reduction_ratio = 0;

    begin = std::chrono::steady_clock::now();
    vertex_cover = vertexCoverHeuristicSLL(&g);
    end = std::chrono::steady_clock::now();

    if (check_solution(&g, vertex_cover)) {
        std::cout << "HeuristicSLL... Done! " << "Time = " \
                  << std::chrono::duration_cast<std::chrono::\
                  microseconds>(end - begin).count() << "[µs]" << std::endl;

        std::ofstream cover_list_sll_file;
        cover_list_sll_file.open("stats/cover_list_sll_file.txt");
        solution_weight = 0;
        for (const vmark &it : vertex_cover) {
            cover_list_sll_file << it << " ";
            solution_weight += g.getVertWeight(it);
        }
        cover_list_sll_file.close();

        std::cout << "HeuristicSLL solution weight is " \
                  << solution_weight << std::endl;

        cover_size = vertex_cover.size();
        std::cout << "HeuristicSLL size : " \
                  << cover_size << std::endl;

        reduction_ratio = \
                        static_cast<float>(cover_size)/graph_size;

        std::cout << "HeuristicSLL rrate : " \
                  << reduction_ratio << std::endl;
    } else {
        std::cout << "HeuristicSLL failed!" << std::endl;
    }

    // HeuristicSLL END

    // HeuristicSPITT

    solution_weight = 0;
    cover_size = 0;
    reduction_ratio = 0;

    begin = std::chrono::steady_clock::now();
    vertex_cover = vertexCoverHeuristicSPITT(&g);
    end = std::chrono::steady_clock::now();

    if (check_solution(&g, vertex_cover)) {
        std::cout << "HeuristicSPITT... Done! " << "Time = " \
                  << std::chrono::duration_cast<std::chrono::\
                  microseconds>(end - begin).count() << "[µs]" << std::endl;

        std::ofstream cover_list_spitt_file;
        cover_list_spitt_file.open("stats/cover_list_spitt_file.txt");
        solution_weight = 0;
        for (const vmark &it : vertex_cover) {
            cover_list_spitt_file << it << " ";
            solution_weight += g.getVertWeight(it);
        }
        cover_list_spitt_file.close();

        std::cout << "HeuristicSPITT solution weight is " \
                  << solution_weight << std::endl;

        cover_size = vertex_cover.size();
        std::cout << "HeuristicSPITT size : " \
                  << cover_size << std::endl;

        reduction_ratio = \
                        static_cast<float>(cover_size)/graph_size;

        std::cout << "HeuristicSPITT rrate : " \
                  << reduction_ratio << std::endl;
    } else {
        std::cout << "HeuristicSPITT failed!" << std::endl;
    }

    // HeuristicSPITT END

    // vertexCoverHeuristicNC

    solution_weight = 0;
    cover_size = 0;
    reduction_ratio = 0;

    std::cout << "HeuristicNC..." << std::endl;

    auto init_sol = vertexCoverHeuristicSLL(&g);
    begin = std::chrono::steady_clock::now();
    auto vertex_cover_wi = vertexCoverHeuristicNC(&g, &init_sol);
    vertex_cover = vertexCoverHeuristicNC(&g, nullptr);
    end = std::chrono::steady_clock::now();

    if (check_solution(&g, vertex_cover) && check_solution(&g, vertex_cover_wi)) {

        std::cout << "HeuristicNC... Done! " << "Time = " \
                  << std::chrono::\
                  duration_cast<std::chrono::microseconds>\
                  (end - begin).count() << "[µs]" << std::endl;

        std::ofstream cover_list_MyAlg_file;
        cover_list_MyAlg_file.open("stats/cover_list_HeuristicNC_file.txt");
        for (const vmark &it : vertex_cover) {
            cover_list_MyAlg_file << it << " ";
        }
        cover_list_MyAlg_file.close();

        auto cover_size_wi = vertex_cover_wi.size();
        cover_size = vertex_cover.size();
        std::cout << "HeuristicNC size : " \
                  << cover_size << std::endl;
        std::cout << "HeuristicNC size with init solution is " \
                  << cover_size_wi << std::endl;

        reduction_ratio = \
                        static_cast<float>(cover_size)/graph_size;

        std::cout << "HeuristicNC rrate : " \
                  << reduction_ratio << std::endl;

        reduction_ratio = \
                        static_cast<float>(cover_size_wi)/graph_size;

        std::cout << "HeuristicNC reduction rate is with init solution " \
                  << reduction_ratio << std::endl;
    } else {
        std::cout << "HeuristicNC failed!" << std::endl;
    }

    // vertexCoverHeuristicNC END

    // GREEDY

    solution_weight = 0;
    cover_size = 0;
    reduction_ratio = 0;

    graph gptr(g);

    std::cout << "Greedy..." << std::endl;

    begin = std::chrono::steady_clock::now();
    gcont greedy_vertex_cover = vertexCoverGreedy(&gptr);
    end = std::chrono::steady_clock::now();

    if (check_solution(&g, greedy_vertex_cover)) {
        std::cout << "Greedy... Done! " << "Time = " \
                  << std::chrono::\
                  duration_cast<std::chrono::microseconds>\
                  (end - begin).count() << "[µs]" << std::endl;

        std::ofstream cover_list_greedy_file;
        cover_list_greedy_file.open("stats/cover_list_greedy.txt");
        for (const vmark &it : greedy_vertex_cover) {
            cover_list_greedy_file << it << " ";
            solution_weight += g.getVertWeight(it);
        }
        cover_list_greedy_file.close();

        std::cout << "Greedy solution weight is " \
                  << solution_weight << std::endl;

        cover_size = greedy_vertex_cover.size();
        std::cout << "GreedyCoverSize size : " \
                  << cover_size << std::endl;

        reduction_ratio = \
                        static_cast<float>(cover_size)/graph_size;

        std::cout << "Greedy rrate : " \
                  << reduction_ratio << std::endl;
    } else {
        std::cout << "Greedy failed!" << std::endl;
    }

    // GREEDY END


    adj_list_file.close();

    std::cout << "Done!" << std::endl;

    return 0;
}
