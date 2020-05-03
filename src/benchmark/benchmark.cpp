#include <random>
#include <iostream>
#include <fstream>
#include <chrono>

#include "../vertex_cover/vertex_cover.h"

#define GRAPH_SIZE 30
#define GRAPH_VERTEX_CONN_PROBABILITY 0.1

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
    std::ofstream cover_list_twoapprox_file;
    std::ofstream cover_list_greedy_file;

    adj_list_file.open("adj_list.txt");
    cover_list_twoapprox_file.open("cover_list_twoapprox.txt");
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

    std::cout << "Calculating graph metrics..." << std::endl;

    int num_of_edges = 0;
    for (auto i = 0; i < graph_size; i++) {
        num_of_edges += g.getAdjNum(i);
    }

    auto avg_num_of_edges = num_of_edges/graph_size;
    std::cout << "Average num of edges is " \
              << avg_num_of_edges << std::endl;

    for (auto i = 0; i < graph_size; i++) {
        gcont adj_list = g.getAdjList(i);
        adj_list_file << i << " ";
        for (auto it = adj_list.begin(); it != adj_list.end(); it++) {
            adj_list_file << *it << " ";
        }
        adj_list_file << std::endl;
    }

    std::cout << "Calculating graph metrics... Done!" << std::endl;

    std::cout << "Generating graph... Done!" << std::endl;

    std::cout << "vertexCoverTwoApproximate..." << std::endl;

    begin = std::chrono::steady_clock::now();
    gcont vertexCoverTwo = vertexCoverTwoApproximate(&g);
    end = std::chrono::steady_clock::now();

    std::cout << "vertexCoverTwoApproximate... Done! " << "Time = " \
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

    for (auto it = vertexCoverTwo.begin(); it != vertexCoverTwo.end(); it++) {
        cover_list_twoapprox_file << *it << " ";
    }

    int TwoApproximateCoverSize = vertexCoverTwo.size();
    std::cout << "vertexCoverTwoApproximate size is " \
              << TwoApproximateCoverSize << std::endl;

    float TwoApproximateReductionRatio = float(TwoApproximateCoverSize)/float(graph_size);

    std::cout << "vertexCoverTwoApproximate reduction rate is " \
              << TwoApproximateReductionRatio << std::endl;

    graph *gptr = &g;

    std::cout << "vertexCoverGreedy..." << std::endl;

    begin = std::chrono::steady_clock::now();
    gcont CoverGreedy = vertexCoverGreedy(gptr);
    end = std::chrono::steady_clock::now();

    std::cout << "vertexCoverGreedy... Done! " << "Time = " \
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

    for (auto it = CoverGreedy.begin(); it != CoverGreedy.end(); it++) {
        cover_list_greedy_file << *it << " ";
    }

    int GreedyCoverSize = CoverGreedy.size();
    std::cout << "GreedyCoverSize size is " \
              << GreedyCoverSize << std::endl;

    float GreedyCoverReductionRatio = float(GreedyCoverSize)/float(graph_size);

    std::cout << "vertexCoverGreedy reduction rate is " \
              << GreedyCoverReductionRatio << std::endl;

    std::cout << "Done!" << std::endl;

    return 0;
}
