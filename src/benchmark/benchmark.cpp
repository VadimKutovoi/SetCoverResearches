#include <random>
#include <iostream>
#include <fstream>

#include "../vertex_cover/vertex_cover.h"

#define GRAPH_SIZE 5
#define GRAPH_VERTEX_CONN_PROBABILITY 1

int main() {
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(0, 100);

    std::ofstream adj_list_file;
    adj_list_file.open("adj_list.txt");

    std::ofstream cover_list_file;
    cover_list_file.open("cover.txt");

    float probability = GRAPH_VERTEX_CONN_PROBABILITY;
    graph g(GRAPH_SIZE);

    std::cout << "Generating graph" << std::endl;

    for (auto i = 0; i < GRAPH_SIZE; i++) {
        for (auto j = 0; j < GRAPH_SIZE; j++) {
            if ( std::floor(dist(e2)) <= probability * 100 && i != j && !g.isConnected(i,j)) {
                g.addEdge(i, j);
            }
        }
    }

    for (auto i = 0; i < GRAPH_SIZE; i++) {
        gcont adj_list = g.getAdjList(i);
        adj_list_file << i << " ";
        for (auto it = adj_list.begin(); it != adj_list.end(); it++) {
            adj_list_file << *it << " ";
        }
        adj_list_file << std::endl;
    }

    std::cout << "vertexCoverTwoApproximate..." << std::endl;

    gcont vertexCoverTwo = vertexCoverTwoApproximate(&g);

    for (auto it = vertexCoverTwo.begin(); it != vertexCoverTwo.end(); it++) {
        cover_list_file << *it << " ";
    }


    //for (auto it = cover.begin(); it != cover.end(); it++)
    //   std::cout << *it << ",";

    std::cout << "vertexCoverTwoApproximate size is " << vertexCoverTwo.size() << std::endl;
    std::cout << std::endl;

    graph *gptr = &g;
    gcont CoverGreedy = vertexCoverGreedy(gptr);

    std::cout << "vertexCoverGreedy size is " << CoverGreedy.size() << std::endl;
    std::cout << std::endl;

    std::cout << "Done!" << std::endl;

    return 0;
}