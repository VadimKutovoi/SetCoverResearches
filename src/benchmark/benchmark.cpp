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

    InputParser input(argc, argv);

    if (input.cmdOptionExists("-s")) {
        graph_size = std::stoi(input.getCmdOption("-s"));
    }

    if (input.cmdOptionExists("-p")) {
        probability = std::stof(input.getCmdOption("-p"));
    }

    if (input.cmdOptionExists("-adjmatrix")) {
        graph_file = input.getCmdOption("-adjmatrix");
    }

    if (input.cmdOptionExists("-type")) {
        graph_type = input.getCmdOption("-type");
    }

    std::cout << "graph_size = " << graph_size << std::endl;
    std::cout << "probability = " << probability << std::endl;

    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution <> dist(0, 100);


    std::ofstream adj_list_file;
    std::ofstream vertex_weights;

    adj_list_file.open("adj_list.txt");
    vertex_weights.open("vertex_weights.txt");

    graph g = generateGridGraph(1000, 50);

    if (!input.cmdOptionExists("-adjmatrix")) {
        std::cout << "Generating graph..." << std::endl;

        std::cout << "Generating basic edges..." << std::endl;

        if (graph_type == "hypercube") {
            std::cout << "Generating hypercube..." << std::endl;
            g = generateHypercubeGraph(graph_size);
        } else if (graph_type == "grid") {
            std::cout << "Generating grid..." << std::endl;
            g = generateGridGraph(graph_size/2, graph_size/2);
        } else if (false) {
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
    } else {
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
    }

    std::cout << "Calculating graph metrics..." << std::endl;

    int num_of_edges = 0;
    for (auto i = 0; i < graph_size; i++) {
        num_of_edges += g.getAdjNum(i);
    }

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

    for (auto i = 0; i < graph_size; i++) {
        gcont adj_list = g.getAdjList(i);
        adj_list_file << i << " ";
        for (auto it = adj_list.begin(); it != adj_list.end(); it++) {
            adj_list_file << *it << " ";
        }
        adj_list_file << std::endl;
    }

    for (auto i = 0; i < graph_size; i++) {
        vmark weight = g.getVertWeight(i);
        vertex_weights << weight << " ";
    }

    std::cout << "Calculating graph metrics... Done!" << std::endl;

    std::cout << "Generating graph... Done!" << std::endl;

    vmark solution_weight = 0;
    vmark cover_size = 0;
    gcont vertex_cover;
    float reduction_ratio = 0;

    graph_size = g.getVertNum();

    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;

    // TWO APPROXIMATE

    solution_weight = 0;
    cover_size = 0;
    reduction_ratio = 0;

    std::cout << "vertexCoverTwoApproximate..." << std::endl;

    begin = std::chrono::steady_clock::now();
    vertex_cover = vertexCoverTwoApproximate(&g);
    end = std::chrono::steady_clock::now();

    std::cout << "vertexCoverTwoApproximate... Done! " << "Time = " \
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
    std::cout << "vertexCoverTwoApproximate size is " \
              << cover_size << std::endl;

    reduction_ratio = \
                    static_cast<float>(cover_size)/graph_size;

    std::cout << "vertexCoverTwoApproximate reduction rate is " \
              << reduction_ratio << std::endl;

    // TWO APPROXIMATE END

    // PRIMAL-DUAL

    solution_weight = 0;
    cover_size = 0;
    reduction_ratio = 0;

    begin = std::chrono::steady_clock::now();
    vertex_cover = vertexCoverPrimalDual(&g);
    end = std::chrono::steady_clock::now();

    std::cout << "vertexCoverPrimalDual... Done! " << "Time = " \
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
    std::cout << "vertexCoverPrimalDual size is " \
              << cover_size << std::endl;

    reduction_ratio = \
                    static_cast<float>(cover_size)/graph_size;

    std::cout << "vertexCoverPrimalDual reduction rate is " \
              << reduction_ratio << std::endl;

    // PRIMAL-DUAL END

    // HeuristicLL

    solution_weight = 0;
    cover_size = 0;
    reduction_ratio = 0;

    begin = std::chrono::steady_clock::now();
    vertex_cover = vertexCoverHeuristicLR(&g);
    end = std::chrono::steady_clock::now();

    std::cout << "vertexCoverHeuristicLR... Done! " << "Time = " \
              << std::chrono::duration_cast<std::chrono::\
              microseconds>(end - begin).count() << "[µs]" << std::endl;

    std::ofstream cover_list_lr_file;
    cover_list_lr_file.open("stats/cover_list_lr_file.txt");
    solution_weight = 0;
    for (const vmark &it : vertex_cover) {
        cover_list_lr_file << it << " ";
        solution_weight += g.getVertWeight(it);
    }
    cover_list_lr_file.close();

    std::cout << "vertexCoverHeuristicLR solution weight is " \
              << solution_weight << std::endl;

    cover_size = vertex_cover.size();
    std::cout << "vertexCoverHeuristicLR size is " \
              << cover_size << std::endl;

    reduction_ratio = \
                    static_cast<float>(cover_size)/graph_size;

    std::cout << "vertexCoverHeuristicLR reduction rate is " \
              << reduction_ratio << std::endl;

    // HeuristicLL END

    // HeuristicLL

    solution_weight = 0;
    cover_size = 0;
    reduction_ratio = 0;

    begin = std::chrono::steady_clock::now();
    vertex_cover = vertexCoverHeuristicLL(&g);
    end = std::chrono::steady_clock::now();

    std::cout << "vertexCoverHeuristicLL... Done! " << "Time = " \
              << std::chrono::duration_cast<std::chrono::\
              microseconds>(end - begin).count() << "[µs]" << std::endl;

    std::ofstream cover_list_ll_file;
    cover_list_ll_file.open("stats/cover_list_ll_file.txt");
    solution_weight = 0;
    for (const vmark &it : vertex_cover) {
        cover_list_ll_file << it << " ";
        solution_weight += g.getVertWeight(it);
    }
    cover_list_ll_file.close();

    std::cout << "vertexCoverHeuristicLL solution weight is " \
              << solution_weight << std::endl;

    cover_size = vertex_cover.size();
    std::cout << "vertexCoverHeuristicLL size is " \
              << cover_size << std::endl;

    reduction_ratio = \
                    static_cast<float>(cover_size)/graph_size;

    std::cout << "vertexCoverHeuristicLL reduction rate is " \
              << reduction_ratio << std::endl;

    // HeuristicLL END

    // HeuristicSLL

    solution_weight = 0;
    cover_size = 0;
    reduction_ratio = 0;

    begin = std::chrono::steady_clock::now();
    vertex_cover = vertexCoverHeuristicSLL(&g);
    end = std::chrono::steady_clock::now();

    std::cout << "vertexCoverHeuristicSLL... Done! " << "Time = " \
              << std::chrono::duration_cast<std::chrono::\
              microseconds>(end - begin).count() << "[µs]" << std::endl;

    std::ofstream cover_list_sll_file;
    cover_list_sll_file.open("stats/cover_list_sll_file.txt");
    solution_weight = 0;
    for (const vmark &it : vertex_cover) {
        cover_list_sll_file << it << " ";
        solution_weight += g.getVertWeight(it);
    }
    cover_list_ll_file.close();

    std::cout << "vertexCoverHeuristicSLL solution weight is " \
              << solution_weight << std::endl;

    cover_size = vertex_cover.size();
    std::cout << "vertexCoverHeuristicSLL size is " \
              << cover_size << std::endl;

    reduction_ratio = \
                    static_cast<float>(cover_size)/graph_size;

    std::cout << "vertexCoverHeuristicSLL reduction rate is " \
              << reduction_ratio << std::endl;

    // HeuristicSLL END

    // GREEDY

    solution_weight = 0;
    cover_size = 0;
    reduction_ratio = 0;

    graph *gptr = &g;

    std::cout << "vertexCoverGreedy..." << std::endl;

    begin = std::chrono::steady_clock::now();
    vertex_cover = vertexCoverGreedy(gptr);
    end = std::chrono::steady_clock::now();

    std::cout << "vertexCoverGreedy... Done! " << "Time = " \
              << std::chrono::\
              duration_cast<std::chrono::microseconds>\
              (end - begin).count() << "[µs]" << std::endl;

    std::ofstream cover_list_greedy_file;
    cover_list_greedy_file.open("stats/cover_list_greedy.txt");
    for (const vmark &it : vertex_cover) {
        cover_list_greedy_file << it << " ";
        solution_weight += g.getVertWeight(it);
    }
    cover_list_greedy_file.close();

    std::cout << "Greedy solution weight is " \
              << solution_weight << std::endl;

    cover_size = vertex_cover.size();
    std::cout << "GreedyCoverSize size is " \
              << cover_size << std::endl;

    reduction_ratio = \
                    static_cast<float>(cover_size)/graph_size;

    std::cout << "vertexCoverGreedy reduction rate is " \
              << reduction_ratio << std::endl;

    // GREEDY END

    adj_list_file.close();

    std::cout << "Done!" << std::endl;

    return 0;
}
