// Copyright 2020 Vadim Kutovoi

#ifndef SRC_GRAPH_GRAPH_H_
#define SRC_GRAPH_GRAPH_H_

#include <list>
#ifdef GRAPH_ENABLE_VERBOSE
#include <iostream>
#endif
#include <algorithm>

#define GRAPH_ERROR -1
#define GRAPH_SUCCESS 0
#define GRAPH_TRUE true
#define GRAPH_FALSE false

class graph {
#define gcont std::list<int>  // defining graph container type
#define vmark int  // defining vertex mark type
    int num_vertices;
    gcont *adj_matrix;

 public:
    explicit graph(int _num_vertices) : num_vertices(_num_vertices) {
        try {
            adj_matrix = new gcont[_num_vertices];
        } catch(std::bad_alloc& ex) {
#ifdef GRAPH_ENABLE_VERBOSE
            std::cout << "Error::graph::graph() can't allocate memory for graph"\
                      << std::endl;
#endif
        }
    }

    ~graph() {
        delete[] adj_matrix;
    }

    vmark addEdge(vmark begin, vmark end);
    vmark getDegree(vmark vertex);
    bool isConnected(vmark begin, vmark end);
    bool isValidVertex(vmark vertex);
};

int graph::addEdge(vmark begin, vmark end) {
    if (!isValidVertex(begin)) {
#ifdef GRAPH_ENABLE_VERBOSE
        std::cout << "Error::graph::addEdge() " << "edge "\
                  << begin << " doesn't exists" << std::endl;
#endif
        return GRAPH_ERROR;
    } else if (!isValidVertex(end)) {
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

vmark graph::getDegree(vmark vertex) {
    if (isValidVertex(vertex)) {
        return adj_matrix[vertex].size();
    }

    return GRAPH_ERROR;
}

bool graph::isConnected(vmark begin, vmark end) {
    if (std::find(adj_matrix[begin].begin(), \
                  adj_matrix[begin].end(), end) != adj_matrix[begin].end() && \
        std::find(adj_matrix[end].begin(), \
                  adj_matrix[end].end(), begin) != adj_matrix[end].end()) {
        return true;
    }
    return false;
}

bool graph::isValidVertex(vmark vertex) {
    if (vertex > num_vertices - 1 || vertex < 0) {
        return GRAPH_FALSE;
    }
    return GRAPH_TRUE;
}

#endif  // SRC_GRAPH_GRAPH_H_
