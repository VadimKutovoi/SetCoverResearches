// Copyright 2020 Vadim Kutovoi

#include <utility>

#include "../graph/graph.h"

#ifndef SRC_VERTEX_COVER_VERTEX_COVER_H_
#define SRC_VERTEX_COVER_VERTEX_COVER_H_

gcont vertexCoverTwoApproximate(const graph *g) {
    gcont vert_cover;
    vmark vert_num = g->getVertNum();

    bool *is_visited = new bool[vert_num];
    for (int i = 0; i < vert_num; i++)
        is_visited[i] = false;

    for (int vert = 0; vert < vert_num; vert++) {
        if (!is_visited[vert]) {
            gcont adj = g->getAdjList(vert);

            for (auto j = adj.begin(); j != adj.end(); j++) {
                vmark adj_vert = *j;

                if (!is_visited[adj_vert]) {
                    is_visited[vert] = true;
                    is_visited[adj_vert] = true;
                    vert_cover.push_back(vert);
                    vert_cover.push_back(adj_vert);
                    break;
                }
            }
        }
    }

    delete[] is_visited;

    return vert_cover;
}

gcont vertexCoverGreedy(graph *g) {
    vmark vert_num = g->getVertNum();
    gcont vert_cover;
    vmark max_weight = -1;
    vmark curr_vert = -1;

    while (true) {
        for (int i = 0; i < vert_num; i++) {
            auto adj_num = g->getAdjNum(i);
            if (adj_num > max_weight) {
                max_weight = adj_num;
                curr_vert = i;
            }
        }

        if (max_weight == 0) break;

        vert_cover.push_back(curr_vert);

        gcont adj = g->getAdjList(curr_vert);
        for (auto j = adj.begin(); j != adj.end(); j++) {
            vmark adj_vert = *j;
            auto err = g->removeEdge(curr_vert, adj_vert);
        }

        max_weight = -1;
    }
    return vert_cover;
}

#endif  // SRC_VERTEX_COVER_VERTEX_COVER_H_
