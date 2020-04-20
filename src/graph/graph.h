// Copyright 2020 Vadim Kutovoi

#ifndef SRC_GRAPH_GRAPH_H_
#define SRC_GRAPH_GRAPH_H_

#include <vector>
#ifdef GRAPH_ENABLE_VERBOSE
#include <iostream>
#endif
#include <algorithm>

#define GRAPH_ERROR -1
#define GRAPH_SUCCESS 0
#define GRAPH_TRUE true
#define GRAPH_FALSE false

#define vmark int  // defining vertex mark type
#define gcont std::vector<vmark>  // defining graph container type

class graph {
    vmark num_vertices;
    vmark *adj_num;
    gcont *adj_matrix;

 public:
    explicit graph(vmark _num_vertices) : num_vertices(_num_vertices) {
        try {
            adj_matrix = new gcont[_num_vertices];
            adj_num = new vmark[_num_vertices];

            for (int i = 0; i < _num_vertices; i++)
                adj_num[i] = 0;
        } catch(std::bad_alloc& ex) {
#ifdef GRAPH_ENABLE_VERBOSE
            std::cout << "Error::" +
                         "graph::graph() can't allocate memory for graph"\
                      << std::endl;
#endif
            throw ex;
        }
    }

    ~graph() {
        delete[] adj_matrix;
        delete[] adj_num;
    }

    vmark addEdge(vmark begin, vmark end);
    vmark getDegree(vmark vertex) const;
    vmark getVertNum() const;
    vmark getAdjNum(vmark vertex) const;
    gcont getAdjList(vmark vertex) const;

    bool isConnected(vmark begin, vmark end) const;
    bool isValidVertex(vmark vertex) const;
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
    adj_num[begin]++;
    adj_num[end]++;

    return GRAPH_SUCCESS;
}

vmark graph::getDegree(vmark vertex) const {
    if (isValidVertex(vertex)) {
        return adj_matrix[vertex].size();
    }

    return GRAPH_ERROR;
}

gcont graph::getAdjList(vmark vertex) const {
    if (isValidVertex(vertex)) {
        return adj_matrix[vertex];
    }
    gcont ret;
    return ret;
}

vmark graph::getAdjNum(vmark vertex) const {
    if (isValidVertex(vertex)) {
        return adj_num[vertex];
    }
    return GRAPH_ERROR;
}

vmark graph::getVertNum() const {
    return this->num_vertices;
}

bool graph::isConnected(vmark begin, vmark end) const {
    if (std::find(adj_matrix[begin].begin(), \
                  adj_matrix[begin].end(), end) != adj_matrix[begin].end() && \
        std::find(adj_matrix[end].begin(), \
                  adj_matrix[end].end(), begin) != adj_matrix[end].end()) {
        return true;
    }
    return false;
}

bool graph::isValidVertex(vmark vertex) const {
    if (vertex > num_vertices - 1 || vertex < 0) {
        return GRAPH_FALSE;
    }
    return GRAPH_TRUE;
}

#endif  // SRC_GRAPH_GRAPH_H_
