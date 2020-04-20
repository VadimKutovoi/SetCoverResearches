// Copyright 2020 Vadim Kutovoi

#include "../graph/graph.h"

#ifndef SRC_VERTEX_COVER_VERTEX_COVER_H_
#define SRC_VERTEX_COVER_VERTEX_COVER_H_

gcont vertexCoverGreedy(const graph *g) {
    vmark vert_num = g->getVertNum();

    bool *is_visited = new bool[vert_num];
    for (int i = 0; i < vert_num; i++)
        is_visited[i] = false;

    for (int j = 0; j < g->getVertNum(); j++) {
        if (!is_visited[j]) {
            // expecting gcont is std container
            gcont adj = g->getAdjList(j);
            for (auto it = adj.begin(); it != adj.end(); it++) {
                vmark adj_vert = *it;
                if (!is_visited[adj_vert]) {
                    is_visited[adj_vert] = true;
                    is_visited[j] = true;
                    break;
                }
            }
        }
    }

    gcont vertex_cover;
    for (int i = 0; i < vert_num; i++) {
        if (is_visited[i]) {
            vertex_cover.push_back(i);
        }
    }

    delete[] is_visited;

    return vertex_cover;
}

#endif  // SRC_VERTEX_COVER_VERTEX_COVER_H_
