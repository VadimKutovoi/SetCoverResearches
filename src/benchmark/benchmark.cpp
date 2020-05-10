#include <random>
#include <iostream>
#include <fstream>
#include <chrono>

#include "../vertex_cover/vertex_cover.h"

#define GRAPH_SIZE 30
#define GRAPH_VERTEX_CONN_PROBABILITY 0.1
#define GRAPH_VERTEX_MAX_WEIGHT_RATIO 1.5 // 

// 1 - graph size, 2 - probability
int main(int argc, char **argv) {
    int graph_size = GRAPH_SIZE;
    float probability = GRAPH_VERTEX_CONN_PROBABILITY;
    if (argc > 1) {
        graph_size = std::stoi(argv[1]);
        if (argc == 3) {
            probability = std::stof(argv[2]);
        }
    }

    std::cout << "graph_size = " << graph_size << std::endl;
    std::cout << "probability = " << probability << std::endl;

    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;

    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution <> dist(0, 100);

    std::ofstream adj_list_file;
    std::ofstream vertex_weights;
    std::ofstream cover_list_twoapprox_file;
    std::ofstream cover_list_primal_dual_file;
    std::ofstream cover_list_greedy_file;

    adj_list_file.open("adj_list.txt");
    vertex_weights.open("vertex_weights.txt");
    cover_list_twoapprox_file.open("cover_list_twoapprox.txt");
    cover_list_primal_dual_file.open("cover_list_primal_dual.txt");
    cover_list_greedy_file.open("cover_list_greedy.txt");

    graph g(graph_size);

    std::cout << "Generating graph..." << std::endl;

    std::cout << "Generating basic edges..." << std::endl;

    for (auto i = 0; i < graph_size; i++) {
        std::cout << "Generating edges for vertex " << i << "\r";
        for (auto j = 0; j < graph_size; j++) {
            if (dist(e2) <= probability * 100 \
                && i != j && !g.isConnected(i, j)) {
                g.addEdge(i, j);
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

    vmark max_weight = graph_size * GRAPH_VERTEX_MAX_WEIGHT_RATIO;
    std::random_device rdmw;
    std::mt19937 emw(rdmw());
    std::uniform_real_distribution <> distmw(1, max_weight);

    for (auto i = 0; i < graph_size; i++) {
        vmark weight = static_cast<int>(distmw(emw));
        g.setVertWeight(i, weight);
    }

    std::cout << "Generating vertex weights... Done!" << std::endl;

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

    // TWO APPROXIMATE

    std::cout << "vertexCoverTwoApproximate..." << std::endl;

    begin = std::chrono::steady_clock::now();
    gcont vertexCoverTwo = vertexCoverTwoApproximate(&g);
    end = std::chrono::steady_clock::now();

    std::cout << "vertexCoverTwoApproximate... Done! " << "Time = " \
              << std::chrono::duration_cast<std::chrono::\
              milliseconds>(end - begin).count() << "[ms]" << std::endl;

    vmark TwoApproxSolWeight = 0;
    for (const vmark &it : vertexCoverTwo) {
        cover_list_twoapprox_file << it << " ";
        TwoApproxSolWeight += g.getVertWeight(it);
    }
    std::cout << "Two approximate solution weight is " \
              << TwoApproxSolWeight << std::endl;


    int TwoAppCoverSize = vertexCoverTwo.size();
    std::cout << "vertexCoverTwoApproximate size is " \
              << TwoAppCoverSize << std::endl;

    float TwoApprReductionRatio = \
                    static_cast<float>(TwoAppCoverSize)/graph_size;

    std::cout << "vertexCoverTwoApproximate reduction rate is " \
              << TwoApprReductionRatio << std::endl;

    // TWO APPROXIMATE END

    // PRIMAL-DUAL

    begin = std::chrono::steady_clock::now();
    gcont vertexPrimalDual = vertexCoverPrimalDual(&g);
    end = std::chrono::steady_clock::now();

    std::cout << "vertexCoverPrimalDual... Done! " << "Time = " \
              << std::chrono::duration_cast<std::chrono::\
              milliseconds>(end - begin).count() << "[ms]" << std::endl;

    vmark PrimalDualSolWeight = 0;
    for (const vmark &it : vertexPrimalDual) {
        cover_list_primal_dual_file << it << " ";
        PrimalDualSolWeight += g.getVertWeight(it);
    }

    std::cout << "PrimalDual solution weight is " \
              << PrimalDualSolWeight << std::endl;

    int PrimalDualCoverSize = vertexPrimalDual.size();
    std::cout << "vertexCoverPrimalDual size is " \
              << PrimalDualCoverSize << std::endl;

    float PrimalDualReductionRatio = \
                    static_cast<float>(PrimalDualCoverSize)/graph_size;

    std::cout << "vertexCoverPrimalDual reduction rate is " \
              << PrimalDualReductionRatio << std::endl;

    // PRIMAL-DUAL END

    // GREEDY

    graph *gptr = &g;

    std::cout << "vertexCoverGreedy..." << std::endl;

    begin = std::chrono::steady_clock::now();
    gcont CoverGreedy = vertexCoverGreedy(gptr);
    end = std::chrono::steady_clock::now();

    std::cout << "vertexCoverGreedy... Done! " << "Time = " \
              << std::chrono::\
              duration_cast<std::chrono::milliseconds>\
              (end - begin).count() << "[ms]" << std::endl;

    vmark GreedySolWeight = 0;
    for (const vmark &it : CoverGreedy) {
        cover_list_greedy_file << it << " ";
        GreedySolWeight += g.getVertWeight(it);
    }

    std::cout << "Greedy solution weight is " \
              << GreedySolWeight << std::endl;

    int GreedyCoverSize = CoverGreedy.size();
    std::cout << "GreedyCoverSize size is " \
              << GreedyCoverSize << std::endl;

    float GreedyCoverReductionRatio = \
                    static_cast<float>(GreedyCoverSize)/graph_size;

    std::cout << "vertexCoverGreedy reduction rate is " \
              << GreedyCoverReductionRatio << std::endl;

    // GREEDY END

    adj_list_file.close();
    cover_list_twoapprox_file.close();
    cover_list_primal_dual_file.close();
    cover_list_greedy_file.close();

    std::cout << "Done!" << std::endl;

    return 0;
}
