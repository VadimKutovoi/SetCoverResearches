// Copyright 2020 Vadim Kutovoi

#include <utility>


#include "../graph/graph.h"

#ifndef SRC_VERTEX_COVER_VERTEX_COVER_H_
#define SRC_VERTEX_COVER_VERTEX_COVER_H_

gcont vertexCoverSimple(const graph *g) {
    gcont vertex_cover;
    vmark vert_num = g->getVertNum();

    bool *is_visited = new bool[vert_num];
    for (int i = 0; i < vert_num; i++)
        is_visited[i] = false;

    for (int i = 0; i < vert_num; i++) {
        if (!is_visited[i]) {
            vertex_cover.push_back(i);
            is_visited[i] = true;

            gcont adj = g->getAdjList(i);
            for (auto j = adj.begin(); j != adj.end(); j++) {
                vmark adj_vert = *j;
                is_visited[adj_vert] = true;
            }
        }
    }

    delete[] is_visited;

    return vertex_cover;
}

gcont vertexCoverGreedy(const graph *g) {
    vmark vert_num = g->getVertNum();
    gcont vertex_cover;

    bool *is_visited = new bool[vert_num];
    for (int i = 0; i < vert_num; i++)
        is_visited[i] = false;

    std::vector<std::pair<vmark, vmark>> vert_weight;

    for (int i = 0; i < vert_num; i++) {
        vert_weight.push_back(std::make_pair(g->getAdjNum(i), i));
    }
    std::sort(vert_weight.begin(), vert_weight.end());

    for (int j = vert_num - 1; j >= 0; j--) {
        vmark curr_vertex = vert_weight[j].second;
        if (!is_visited[curr_vertex]) {
            vertex_cover.push_back(curr_vertex);
            is_visited[curr_vertex] = true;

            gcont adj = g->getAdjList(curr_vertex);
            for (auto it = adj.begin(); it != adj.end(); it++) {
                vmark adj_vert = *it;
                is_visited[adj_vert] = true;
            }
        }
    }

    delete[] is_visited;

    return vertex_cover;
}

#endif  // SRC_VERTEX_COVER_VERTEX_COVER_H_
