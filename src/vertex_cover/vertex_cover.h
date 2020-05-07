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

            for (const vmark &adj_vert : adj) {
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
    float max_profit = -1;
    vmark curr_vert = -1;

    while (true) {
        for (int i = 0; i < vert_num; i++) {
            float profit = static_cast<float>(g->getAdjNum(i)) / \
                                              g->getVertWeight(i);
            if (profit > max_profit) {
                max_profit = profit;
                curr_vert = i;
            }
        }

        if (max_profit == 0) break;

        vert_cover.push_back(curr_vert);

        gcont adj = g->getAdjList(curr_vert);
        for (const vmark &adj_vert : adj) {
            auto err = g->removeEdge(curr_vert, adj_vert);
        }

        max_profit = -1;
    }
    return vert_cover;
}

gcont vertexCoverPrimalDual(graph *g) {
    gcont vert_cover;
    auto vert_num = g->getVertNum();
    vmark *vert_edge_sum = new vmark[vert_num];
    bool *in_cover = new bool[vert_num];

    for (int i = 0; i < vert_num; i++) {
        vert_edge_sum[i] = 0;
        in_cover[i] = false;
    }

    for (vmark curr_vertex = 0; curr_vertex < vert_num; curr_vertex++) {
        auto adj_list = g->getAdjList(curr_vertex);

        if (g->getVertWeight(curr_vertex) \
                             - vert_edge_sum[curr_vertex] == 0) continue;

        for (const vmark &adj_vertex : adj_list) {
            if (adj_vertex > curr_vertex) {
                float delta = \
                    std::min(g->getVertWeight(curr_vertex) \
                             - vert_edge_sum[curr_vertex], \
                             g->getVertWeight(adj_vertex) \
                             - vert_edge_sum[adj_vertex]);

                if (delta == 0) continue;

                vert_edge_sum[curr_vertex] += delta;
                vert_edge_sum[adj_vertex] += delta;

                if (vert_edge_sum[curr_vertex] ==\
                    g->getVertWeight(curr_vertex) && !in_cover[curr_vertex]) {
                    vert_cover.push_back(curr_vertex);
                    in_cover[curr_vertex] = true;
                }

                if (vert_edge_sum[adj_vertex] ==\
                    g->getVertWeight(adj_vertex) && !in_cover[adj_vertex]) {
                    vert_cover.push_back(adj_vertex);
                    in_cover[adj_vertex] = true;
                }
            }
        }
    }

    delete[] in_cover;
    delete[] vert_edge_sum;
    return vert_cover;
}

#endif  // SRC_VERTEX_COVER_VERTEX_COVER_H_
