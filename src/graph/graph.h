// Copyright 2020 Vadim Kutovoi

#ifndef SRC_GRAPH_GRAPH_H_
#define SRC_GRAPH_GRAPH_H_

#include <list>
#ifdef GRAPH_ENABLE_VERBOSE
#include <iostream>
#endif
#include <algorithm>

#define GRAPH_ERROR 1
#define GRAPH_SUCCESS 0

class graph {
    int num_vertices;
    std::list<int> *adj_matrix;
 public:
    explicit graph(int _num_vertices) : num_vertices(_num_vertices) {
        try {
            adj_matrix = new std::list<int>[_num_vertices];
        } catch(std::bad_alloc& ex) {
#ifdef GRAPH_ENABLE_VERBOSE
            std::cout << "Error::graph::graph() can't allocate memory for graph" << std::endl;
#endif
        }
    }

    ~graph() {
        delete[] adj_matrix;
    }

    int addEdge(int begin, int end);
    bool isConnected(int begin, int end);
};

int graph::addEdge(int begin, int end) {
    if (begin > num_vertices - 1) {
#ifdef GRAPH_ENABLE_VERBOSE
        std::cout << "Error::graph::addEdge() " << "edge "\
                  << begin << " doesn't exists" << std::endl;
#endif
        return GRAPH_ERROR;
    } else if (end > num_vertices - 1) {
#ifdef GRAPH_ENABLE_VERBOSE
        std::cout << "Error::graph::addEdge() " << "edge "\
                  << end << " doesn't exists" << std::endl;
#endif
        return GRAPH_ERROR;
    }

    if (std::find(adj_matrix[begin].begin(), \
                  adj_matrix[begin].end(), end) != adj_matrix[begin].end() || \
        std::find(adj_matrix[end].begin(),
                  adj_matrix[end].end(), begin) != adj_matrix[end].end()) {
#ifdef GRAPH_ENABLE_VERBOSE
        std::cout << "Error::graph::addEdge() " << "edge "\
                  << begin << "-" << end << " already exists" << std::endl;
#endif
        return GRAPH_ERROR;
    }

    adj_matrix[begin].push_back(end);
    adj_matrix[end].push_back(begin);

    return GRAPH_SUCCESS;
}

bool graph::isConnected(int begin, int end) {
    if (std::find(adj_matrix[begin].begin(), \
                  adj_matrix[begin].end(), end) != adj_matrix[begin].end() && \
        std::find(adj_matrix[end].begin(), \
                  adj_matrix[end].end(), begin) != adj_matrix[end].end()) {
        return true;
    }
    return false;
}

#endif  // SRC_GRAPH_GRAPH_H_
