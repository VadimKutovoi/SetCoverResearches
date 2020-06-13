// Copyright 2020 Vadim Kutovoi

#include <utility>
#include <random>

#include "../graph/graph.h"

#ifndef SRC_VERTEX_COVER_VERTEX_COVER_H_
#define SRC_VERTEX_COVER_VERTEX_COVER_H_

#define VCOVER_HEURISTIC_LR 1
#define VCOVER_HEURISTIC_ED 2
#define VCOVER_HEURISTIC_SPITT 3
#define VCOVER_HEURISTIC_LL 4
#define VCOVER_HEURISTIC_SLL 5
#define VCOVER_HEURISTIC_ASLL 6

gcont vertexCoverTwoApproximate(const graph *g, vmark start_v = 0) {
    gcont vert_cover;
    vmark vert_num = g->getVertNum();

    bool *is_visited = new bool[vert_num];
    for (int i = 0; i < vert_num; i++)
        is_visited[i] = false;

    for (int vert = start_v; vert < vert_num; vert++) {
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

    if (start_v) {
        std::cout << "TA: start_v = " << start_v << std::endl;
        for (int vert = 0; vert < start_v; vert++) {
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
    }

    delete[] is_visited;

    return vert_cover;
}

gcont vertexCoverHeuristicLD(const graph *g) {
    gcont vertex_cover;
    vmark vert_num = g->getVertNum();
    std::vector<std::pair<vmark, vmark>> decr_list;
    bool *in_cover = new bool[vert_num];

    for (int curr_vert = 0; curr_vert < vert_num; curr_vert++) {
        decr_list.push_back(std::make_pair(g->getAdjNum(curr_vert), curr_vert));
    }

    std::sort(decr_list.begin(), decr_list.end());

    for (int i = 0; i < vert_num; i++)
        in_cover[i] = false;


    bool found = false;

    for (int i = vert_num - 1; i >= 0; i--) {
        auto curr_vert = decr_list[i].second;

        if (!in_cover[curr_vert]) {
            for (const vmark &adj_vert : g->getAdjList(curr_vert)) {
                if (!in_cover[adj_vert]) {
                    for (const vmark &adj_vert2 : g->getAdjList(adj_vert)) {
                        if (!in_cover[adj_vert2]) {
                            found = true;
                            break;
                        }
                    }
                    if (found) {
                        vertex_cover.push_back(curr_vert);
                        in_cover[curr_vert] = true;
                        found = false;
                        break;
                    }
                }
            }
        }
    }

    delete[] in_cover;
    return vertex_cover;
}

gcont vertexCoverHeuristicLR(const graph *g) {
    gcont vertex_cover;
    vmark vert_num = g->getVertNum();
    std::vector<std::pair<vmark, vmark>> decr_list;

    auto rd = std::random_device {};
    auto rng = std::default_random_engine { rd() };

    bool *in_cover = new bool[vert_num];

    for (int curr_vert = 0; curr_vert < vert_num; curr_vert++) {
        decr_list.push_back(std::make_pair(g->getAdjNum(curr_vert), curr_vert));
    }

    std::shuffle(std::begin(decr_list), std::end(decr_list), rng);

    for (int i = 0; i < vert_num; i++)
        in_cover[i] = false;

    bool found = false;

    for (int i = 0; i < vert_num; i++) {
        auto curr_vert = decr_list[i].second;

        if (!in_cover[curr_vert]) {
            for (const vmark &adj_vert : g->getAdjList(curr_vert)) {
                if (!in_cover[adj_vert] && g->getVertWeight(curr_vert) <= g->getVertWeight(adj_vert)) {
                        vertex_cover.push_back(curr_vert);
                        in_cover[curr_vert] = true;
                        break;
                }
            }
        }
    }

    delete[] in_cover;
    return vertex_cover;
}

gcont vertexCoverHeuristicLR2(const graph *g) {
    gcont vertex_cover;
    vmark vert_num = g->getVertNum();
    std::vector<std::pair<vmark, vmark>> decr_list;
    bool *in_cover = new bool[vert_num];

    for (int curr_vert = 0; curr_vert < vert_num; curr_vert++) {
        decr_list.push_back(std::make_pair(g->getAdjNum(curr_vert), curr_vert));
    }

    std::sort(decr_list.begin(), decr_list.end());

    for (int i = 0; i < vert_num; i++)
        in_cover[i] = false;

    bool found = false;

    for (int i = 0; i < vert_num; i++) {
        auto curr_vert = decr_list[i].second;

        if (!in_cover[curr_vert]) {
            for (const vmark &adj_vert : g->getAdjList(curr_vert)) {
                if (!in_cover[adj_vert] && g->getVertWeight(curr_vert) <= g->getVertWeight(adj_vert)) {
                        vertex_cover.push_back(curr_vert);
                        in_cover[curr_vert] = true;
                        break;
                }
            }
        }
    }

    delete[] in_cover;
    return vertex_cover;
}

gcont vertexCoverHeuristicSLL(const graph *g) {
    gcont vertex_cover;
    vmark vert_num = g->getVertNum();
    std::vector<std::pair<vmark, vmark>> decr_list;
    bool *in_cover = new bool[vert_num];

    for (int curr_vert = 0; curr_vert < vert_num; curr_vert++) {
        decr_list.push_back(std::make_pair(g->getAdjNum(curr_vert), curr_vert));
    }

    std::sort(decr_list.begin(), decr_list.end());

    for (int i = 0; i < vert_num; i++)
        in_cover[i] = false;


    bool found = false;

    for (int i = vert_num - 1; i >= 0; i--) {
        auto curr_vert = decr_list[i].second;

        if (!in_cover[curr_vert]) {
            for (const vmark &adj_vert : g->getAdjList(curr_vert)) {
                if (!in_cover[adj_vert] && g->getVertWeight(curr_vert) <= g->getVertWeight(adj_vert)) {
                        vertex_cover.push_back(curr_vert);
                        in_cover[curr_vert] = true;
                        break;
                }
            }
        }
    }

    delete[] in_cover;
    return vertex_cover;
}

gcont vertexCoverHeuristicED(const graph *g) {
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

// ListLeft
gcont vertexCoverHeuristicLL(const graph *g) {
    gcont vertex_cover;
    vmark vert_num = g->getVertNum();

    for (int curr_vert = 0; curr_vert < vert_num; curr_vert++) {
        for (const vmark &adj_vert : g->getAdjList(curr_vert)) {
            if (curr_vert < adj_vert) {
                vertex_cover.push_back(curr_vert);
                break;
            }
        }
    }

    return vertex_cover;
}


// Sorted-LL
gcont vertexCoverHeuristicSLL2(const graph *g) {
    gcont vertex_cover;
    vmark vert_num = g->getVertNum();

    for (int curr_vert = 0; curr_vert < vert_num; curr_vert++) {
        for (const vmark &adj_vert : g->getAdjList(curr_vert)) {
            if (g->getAdjNum(adj_vert) <= g->getAdjNum(curr_vert) || \
                curr_vert < adj_vert && \
                g->getAdjNum(adj_vert) == g->getAdjNum(curr_vert)) {
                vertex_cover.push_back(curr_vert);
                break;
            }
        }
    }

    return vertex_cover;
}

// Anti Sorted-LL
gcont vertexCoverHeuristicASLL(const graph *g) {
    gcont vertex_cover;
    vmark vert_num = g->getVertNum();

    for (int curr_vert = 0; curr_vert < vert_num; curr_vert++) {
        for (const vmark &adj_vert : g->getAdjList(curr_vert)) {
            if (g->getAdjNum(adj_vert) >= g->getAdjNum(curr_vert) && \
                adj_vert < curr_vert) {
                vertex_cover.push_back(curr_vert);
                break;
            }
        }
    }

    return vertex_cover;
}

gcont vertexCoverHeuristicSPITT(const graph *g) {
    gcont vertex_cover;
    vmark vert_num = g->getVertNum();
    bool *in_cover = new bool[vert_num];

    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution <> dist(0, 1);


    for (int i = 0; i < vert_num; i++)
        in_cover[i] = false;

    for (int curr_vert = 0; curr_vert < vert_num; curr_vert++) {
        if (!in_cover[curr_vert]) {
            for (const vmark &adj_vert : g->getAdjList(curr_vert)) {
                if (!in_cover[adj_vert]) {
                    if (dist(e2) <= 0.5) {
                        in_cover[curr_vert] = true;
                        vertex_cover.push_back(curr_vert);
                        break;
                    } else {
                        in_cover[adj_vert] = true;
                        vertex_cover.push_back(adj_vert);
                    }
                }
            }
        }
    }

    delete[] in_cover;

    return vertex_cover;
}

gcont vertexCoverHeuristicSPITTW(const graph *g) {
    gcont vertex_cover;
    vmark vert_num = g->getVertNum();
    bool *in_cover = new bool[vert_num];

    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution <> dist(0, 1);


    for (int i = 0; i < vert_num; i++)
        in_cover[i] = false;

    for (int curr_vert = 0; curr_vert < vert_num; curr_vert++) {
        if (!in_cover[curr_vert]) {
            for (const vmark &adj_vert : g->getAdjList(curr_vert)) {
                if (!in_cover[adj_vert]) {
                    if (dist(e2) <= 0.5) {
                        in_cover[curr_vert] = true;
                        vertex_cover.push_back(curr_vert);
                        break;
                    } else {
                        in_cover[adj_vert] = true;
                        vertex_cover.push_back(adj_vert);
                    }
                }
            }
        }
    }

    delete[] in_cover;

    return vertex_cover;
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

gcont vertexCoverHeuristicNC(graph *g, gcont *c = nullptr) {
    gcont vert_cover;
    auto vert_num = g->getVertNum();
    bool *in_cover = new bool[vert_num];

    // Thinking what all graph is cover and removing all what we can
    if (c == nullptr) {
        for (int i = 0; i < vert_num; i++)
            in_cover[i] = true;
    } else {
        for (int i = 0; i < vert_num; i++)
            in_cover[i] = false;

        for (const vmark &covered_vertex : *c)
            in_cover[covered_vertex] = true;
    }

    for (auto curr_vertex = 0; curr_vertex < vert_num; curr_vertex++) {
        auto adj_list = g->getAdjList(curr_vertex);

        bool is_all_adj_cov = true;
        for (const vmark &adj_vertex : adj_list) {
            if (!in_cover[adj_vertex]) is_all_adj_cov = false;
        }

        if (is_all_adj_cov) in_cover[curr_vertex] = false;
    }

    for (int i = 0; i < vert_num; i++)
        if (in_cover[i]) vert_cover.push_back(i);

    delete[] in_cover;
    return vert_cover;
}

#endif  // SRC_VERTEX_COVER_VERTEX_COVER_H_
