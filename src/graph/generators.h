// Copyright 2020 Vadim Kutovoi

#ifndef SRC_GRAPH_GENERATORS_H_
#define SRC_GRAPH_GENERATORS_H_

#include "graph.h"
#include <iostream>
#include <cmath>

graph generateGridGraph(vmark width, vmark height) {
    vmark num_nodes = width * height;
    graph g(num_nodes);

    for (auto column = 0; column < width; column++) {
        for (auto row = 0; row < height; row++) {
            vmark curr_vertex = row + column * height;

            if (column != width - 1) {
                g.addEdge(curr_vertex, curr_vertex + height);
            }

            if (row != height - 1) {
                g.addEdge(curr_vertex, curr_vertex + 1);
            }
        }
    }

    return g;
}

vmark hammingDistance(vmark n1, vmark n2) {
    vmark x = n1 ^ n2;
    vmark setBits = 0;

    while (x > 0) {
        setBits += x & 1;
        x >>= 1;
    }

    return setBits;
}

graph generateHypercubeGraph(vmark num_vertices) {
    graph g(num_vertices);

    for (auto vertex1 = 0; vertex1 < num_vertices; vertex1++) {
        for (auto vertex2 = vertex1 + 1; vertex2 < num_vertices; vertex2++) {
            if (vertex1 != vertex2 && hammingDistance(vertex1, vertex2) == 1) {
                g.addEdge(vertex1, vertex2);
            }
        }
    }

    return g;
}

bool isBitSet(vmark num, vmark bit) {
    if (num & (1 << bit))
        return true;
    else
        return false;
}

graph generateCompBipartiteGraph(vmark part_num_nodes) {
    vmark num_nodes = part_num_nodes * 2;
    graph g(num_nodes);

    for (auto vertex1 = 0; vertex1 < part_num_nodes; vertex1++) {
        for (auto vertex2 = part_num_nodes; vertex2 < num_nodes; vertex2++) {
            g.addEdge(vertex1, vertex2);
        }
    }

    return g;
}

graph generateButterflyGraph(vmark num_dims) {
    vmark width = num_dims + 1;
    vmark height = std::pow(2, num_dims);
    graph g(width * height);

    for (auto i = 0; i < width - 1; i++) {
        for (auto w = 0; w < height; w++) {
            vmark vertex1 = w + i * height;
            std::cout << "vertex1 = " << vertex1 << std::endl;
            vmark i_2 = i + 1;
            for (auto w_2 = 0; w_2 < height; w_2++) {
                vmark vertex2 = w_2 + i_2 * height;
                std::cout << "vertex2 = " << vertex2 << std::endl;

                bool b1 = isBitSet(w, i);
                bool b2 = isBitSet(w_2, i);

                if (w == w_2 || b1 == b2) {
                    std::cout << "addEdge(" << vertex1 << ", " << vertex2 << ")" << std::endl;
                    g.addEdge(vertex1, vertex2);
                }
            }
        }
    }

    return g;
}

#endif  // SRC_GRAPH_GENERATORS_H_
