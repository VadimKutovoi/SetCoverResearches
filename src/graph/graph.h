// Copyright 2020 Vadim Kutovoi

#ifndef SRC_GRAPH_GRAPH_H_
#define SRC_GRAPH_GRAPH_H_
#define GRAPH_ENABLE_VERBOSE
#include <vector>
#include <algorithm>
#ifdef GRAPH_ENABLE_VERBOSE
#include <iostream>
#endif

#define GRAPH_ERROR -1
#define GRAPH_SUCCESS 0
#define GRAPH_TRUE true
#define GRAPH_FALSE false

#define gerror short
#define gbool bool
#define vmark unsigned int  // defining vertex mark type
#define gcont std::vector<vmark>  // defining graph container type

class graph {
    vmark num_vertices;
    vmark *adj_num;
    vmark *vert_weight;
    gcont *adj_matrix;

 public:
    explicit graph(vmark _num_vertices) : num_vertices(_num_vertices) {
        try {
            adj_matrix = new gcont[_num_vertices];
            adj_num = new vmark[_num_vertices];
            vert_weight = new vmark[_num_vertices];

            for (int i = 0; i < _num_vertices; i++) {
                adj_num[i] = 0;
                vert_weight[i] = 1;
            }
        } catch(std::bad_alloc& ex) {
#ifdef GRAPH_ENABLE_VERBOSE
            std::cout << "Error::\
                         graph::graph() can't allocate memory for graph"\
                      << std::endl;
#endif
            throw ex;
        }
    }

    graph(const graph &g) : num_vertices(g.num_vertices) {
        graph new_graph(num_vertices);

        adj_matrix = new gcont[num_vertices];
        adj_num = new vmark[num_vertices];
        vert_weight = new vmark[num_vertices];

        for (int i = 0; i < num_vertices; i++) {
            adj_num[i] = g.adj_num[i];
            vert_weight[i] = g.vert_weight[i];
            adj_matrix[i] = g.adj_matrix[i];
        }
    }

    ~graph() {
        delete[] vert_weight;
        delete[] adj_matrix;
        delete[] adj_num;
    }

    gerror addEdge(vmark begin, vmark end);
    gerror removeEdge(vmark begin, vmark end);
    vmark getDegree(vmark vertex) const;
    vmark getVertNum() const;
    vmark setVertWeight(vmark vertex, vmark weight);
    vmark getVertWeight(vmark vertex) const;
    vmark getAdjNum(vmark vertex) const;
    gcont getAdjList(vmark vertex) const;

    bool isConnected(vmark begin, vmark end) const;
    bool isValidVertex(vmark vertex) const;
};

gerror graph::addEdge(vmark begin, vmark end) {
#ifndef GRAPH_DISABLE_CHECK
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
#endif

    adj_matrix[begin].push_back(end);
    adj_matrix[end].push_back(begin);
    adj_num[begin]++;
    adj_num[end]++;

    return GRAPH_SUCCESS;
}

gerror graph::removeEdge(vmark begin, vmark end) {
#ifndef GRAPH_DISABLE_CHECK
    if (!isValidVertex(begin)) {
#ifdef GRAPH_ENABLE_VERBOSE
        std::cout << "Error::graph::removeEdge() " << "edge "\
                  << begin << " doesn't exists" << std::endl;
#endif
        return GRAPH_ERROR;
    } else if (!isValidVertex(end)) {
#ifdef GRAPH_ENABLE_VERBOSE
        std::cout << "Error::graph::removeEdge() " << "edge "\
                  << end << " doesn't exists" << std::endl;
#endif
        return GRAPH_ERROR;
    }
#endif

    auto itend = std::find(adj_matrix[begin].begin(),
                         adj_matrix[begin].end(), end);
    auto itbegin = std::find(adj_matrix[end].begin(),
                         adj_matrix[end].end(), begin);

    if (itend != adj_matrix[begin].end() && itbegin != adj_matrix[end].end()) {
        adj_matrix[begin].erase(itend);
        adj_matrix[end].erase(itbegin);

        adj_num[begin]--;
        adj_num[end]--;

        return GRAPH_SUCCESS;
    }

#ifdef GRAPH_ENABLE_VERBOSE
    std::cout << "Error::graph::removeEdge() " << "edge "\
              << begin << "-" << end << " doesn't exists" << std::endl;
#endif

    return GRAPH_ERROR;
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

vmark graph::setVertWeight(vmark vertex, vmark weight) {
    if (isValidVertex(vertex)) {
        this->vert_weight[vertex] = weight;
        return GRAPH_SUCCESS;
    }
    return GRAPH_ERROR;
}

vmark graph::getVertWeight(vmark vertex) const {
    if (isValidVertex(vertex)) {
        return this->vert_weight[vertex];
    }
    return GRAPH_ERROR;
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
