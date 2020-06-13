// Copyright 2020 Vadim Kutovoi

#include "../src/graph/graph.h"
#include "../src/graph/generators.h"
#include "../src/vertex_cover/vertex_cover.h"
#include <gtest/gtest.h>

TEST(GraphTest, CanAddEdge) {
    graph g(2);
    EXPECT_EQ(g.addEdge(0, 1), GRAPH_SUCCESS);
}

TEST(GraphTest, CanAddEdgeIfVertIsNegative) {
    graph g(2);
    EXPECT_EQ(g.addEdge(-1, 1), GRAPH_ERROR);
}

TEST(GraphTest, CantAddEdgeToNonExistVert) {
    graph g(2);
    EXPECT_EQ(g.addEdge(0, 3), GRAPH_ERROR);
}

TEST(GraphTest, ExistingVertIsConnected) {
    graph g(2);
    g.addEdge(0, 1);
    EXPECT_TRUE(g.isConnected(0, 1));
}

TEST(GraphTest, NonExistingVertIsNotConnected) {
    graph g(2);

    EXPECT_FALSE(g.isConnected(0, 1));
}

TEST(GraphTest, CanRemoveEdge) {
    graph g(3);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.removeEdge(0, 1);

    EXPECT_FALSE(g.isConnected(0, 1));
}

TEST(GraphTest, CantRemoveNonExistingEdge) {
    graph g(3);
    g.addEdge(0, 1);

    EXPECT_EQ(g.removeEdge(0, 2), GRAPH_ERROR);
}

TEST(GraphTest, RemoveIsBidirectional) {
    graph g(2);
    g.addEdge(0, 1);
    g.removeEdge(1, 0);

    EXPECT_EQ(g.getAdjNum(1) , 0);
    EXPECT_EQ(g.getAdjNum(0) , 0);
}

TEST(GraphTest, ReturnsRightDegree) {
    int expected_degree = 2;
    graph g(expected_degree + 1);

    g.addEdge(0, 1);
    g.addEdge(0, 2);

    EXPECT_EQ(g.getDegree(0), expected_degree);
}

TEST(GraphTest, getDegreeReturnsErrorIfVertNotExist) {
    graph g(1);
    EXPECT_EQ(g.getDegree(1), GRAPH_ERROR);
}

TEST(GraphTest, getDegreeReturnsErrorIfVertNegative) {
    graph g(1);
    EXPECT_EQ(g.getDegree(-1), GRAPH_ERROR);
}

TEST(GraphTest, GetAdjNumIsCorrect) {
    graph g(6);

    /*     1-----3
          /|    /
         / |   /
        0  |  /     5
         \ | /     /
          \|/     /
           2-----4
    */

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(2, 4);
    g.addEdge(4, 5);

    EXPECT_EQ(g.getAdjNum(0), 2);
    EXPECT_EQ(g.getAdjNum(1), 3);
    EXPECT_EQ(g.getAdjNum(2), 4);
    EXPECT_EQ(g.getAdjNum(3), 2);
    EXPECT_EQ(g.getAdjNum(4), 2);
    EXPECT_EQ(g.getAdjNum(5), 1);
}

TEST(GraphTest, GetAdjNumIsCorrectAfterRemovingAllEdges) {
    graph g(4);

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(0, 3);

    g.removeEdge(0, 1);
    g.removeEdge(0, 2);
    g.removeEdge(0, 3);

    EXPECT_EQ(g.getAdjNum(0), 0);
    EXPECT_EQ(g.getAdjNum(1), 0);
    EXPECT_EQ(g.getAdjNum(2), 0);
    EXPECT_EQ(g.getAdjNum(2), 0);
}

TEST(GraphTest, GetAdjListIsCorrect) {
    graph g(2);

    g.addEdge(0, 1);

    EXPECT_EQ(g.getAdjList(0)[0], 1);
}

TEST(GraphTest, GetAdjListHasExpectedOrdering) {
    graph g(4);

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(0, 3);

    EXPECT_EQ(g.getAdjList(0)[0], 1);
    EXPECT_EQ(g.getAdjList(0)[1], 2);
    EXPECT_EQ(g.getAdjList(0)[2], 3);
}

TEST(GraphTest, CanGetNumVertices) {
    graph g(5);

    EXPECT_EQ(g.getVertNum(), 5);
}

TEST(GraphTest, ValidVertexIsValid) {
    graph g(1);
    EXPECT_TRUE(g.isValidVertex(0));
}

TEST(GraphTest, InvalidVertexIsInvalid) {
    graph g(1);
    EXPECT_FALSE(g.isValidVertex(2));
}

TEST(GraphTest, CanSetAndGetVertWeight) {
    graph g(1);
    g.setVertWeight(0, 10);
    EXPECT_EQ(g.getVertWeight(0), 10);
}

TEST(GraphTest, UninitVertWeightIsOne) {
    graph g(1);
    EXPECT_EQ(g.getVertWeight(0), 1);
}

TEST(GraphTest, CopyConstructorWorks) {
    graph g1(2);
    g1.addEdge(0, 1);
    graph g2 = g1;
    EXPECT_EQ(g2.isConnected(0, 1), g1.isConnected(0, 1));
    EXPECT_EQ(g2.getVertNum(), g1.getVertNum());
}

TEST(VertexCoverTest, vertexCoverTwoApproximate) {
    graph g(6);

    /*     1-----3
          /|    /
         / |   /
        0  |  /     5
         \ | /     /
          \|/     /
           2-----4
    */

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(2, 4);
    g.addEdge(4, 5);

    gcont vertex_cover = vertexCoverTwoApproximate(&g);

    EXPECT_EQ(vertex_cover[0], 0);
    EXPECT_EQ(vertex_cover[1], 1);
    EXPECT_EQ(vertex_cover[2], 2);
    EXPECT_EQ(vertex_cover[3], 3);
    EXPECT_EQ(vertex_cover[4], 4);
    EXPECT_EQ(vertex_cover[5], 5);
}

TEST(VertexCoverTest, vertexCoverHeuristicLR) {
    graph g(6);

    /*     1-----3
          /|    /
         / |   /
        0  |  /     5
         \ | /     /
          \|/     /
           2-----4
    */

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(2, 4);
    g.addEdge(4, 5);

    gcont vertex_cover = vertexCoverHeuristicLR(&g);

    EXPECT_EQ(vertex_cover[0], 1);
    EXPECT_EQ(vertex_cover[1], 2);
    EXPECT_EQ(vertex_cover[2], 5);
}

TEST(VertexCoverTest, vertexCoverHeuristicLL) {
    graph g(6);

    /*     1-----3
          /|    /
         / |   /
        0  |  /     5
         \ | /     /
          \|/     /
           2-----4
    */

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(2, 4);
    g.addEdge(4, 5);

    gcont vertex_cover = vertexCoverHeuristicLL(&g);

    EXPECT_EQ(vertex_cover[0], 0);
    EXPECT_EQ(vertex_cover[1], 1);
    EXPECT_EQ(vertex_cover[2], 2);
    EXPECT_EQ(vertex_cover[3], 4);
}

TEST(VertexCoverTest, vertexCoverHeuristicSLL) {
    graph g(6);

    /*     1-----3
          /|    /
         / |   /
        0  |  /     5
         \ | /     /
          \|/     /
           2-----4
    */

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(2, 4);
    g.addEdge(4, 5);

    gcont vertex_cover = vertexCoverHeuristicSLL(&g);

    EXPECT_EQ(vertex_cover[0], 1);
    EXPECT_EQ(vertex_cover[1], 2);
    EXPECT_EQ(vertex_cover[2], 4);
}

TEST(VertexCoverTest, vertexCoverHeuristicASLL) {
    graph g(6);

    /*     1-----3
          /|    /
         / |   /
        0  |  /     5
         \ | /     /
          \|/     /
           2-----4
    */

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(2, 4);
    g.addEdge(4, 5);

    gcont vertex_cover = vertexCoverHeuristicASLL(&g);

    std::cout << vertex_cover.size() << std::endl;

    EXPECT_EQ(vertex_cover[0], 1);
    EXPECT_EQ(vertex_cover[1], 2);
    EXPECT_EQ(vertex_cover[2], 4);
}

TEST(VertexCoverTest, vertexCoverHeuristicSPITT) {
    graph g(6);

    /*     1-----3
          /|    /
         / |   /
        0  |  /     5
         \ | /     /
          \|/     /
           2-----4
    */

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(2, 4);
    g.addEdge(4, 5);

    g.setVertWeight(0, 5);
    g.setVertWeight(1, 2);
    g.setVertWeight(2, 7);
    g.setVertWeight(3, 1);
    g.setVertWeight(4, 10);
    g.setVertWeight(5, 8);

    gcont vertex_cover = vertexCoverHeuristicSPITT(&g);

    for (const auto &i : vertex_cover)
        std::cout << i << std::endl;

    EXPECT_EQ(vertex_cover[0], 1);
    EXPECT_EQ(vertex_cover[1], 0);
    EXPECT_EQ(vertex_cover[2], 3);
    EXPECT_EQ(vertex_cover[3], 2);
    EXPECT_EQ(vertex_cover[4], 4);
}

TEST(VertexCoverTest, Greedy) {
    graph g(6);
    graph *gptr = &g;

    /*     1-----3
          /|    /
         / |   /
        0  |  /     5
         \ | /     /
          \|/     /
           2-----4
    */

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(2, 4);
    g.addEdge(4, 5);

    gcont vertex_cover = vertexCoverGreedy(gptr);

    EXPECT_EQ(vertex_cover[0], 2);
    EXPECT_EQ(vertex_cover[1], 1);
    EXPECT_EQ(vertex_cover[2], 4);
}

TEST(VertexCoverTest, PrimalDual) {
    graph g(6);

    /*     1-----3
          /|    /
         / |   /
        0  |  /     5
         \ | /     /
          \|/     /
           2-----4
    */

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(2, 4);
    g.addEdge(4, 5);

    g.setVertWeight(0, 5);
    g.setVertWeight(1, 2);
    g.setVertWeight(2, 7);
    g.setVertWeight(3, 1);
    g.setVertWeight(4, 10);
    g.setVertWeight(5, 8);

    gcont vertex_cover = vertexCoverPrimalDual(&g);

    EXPECT_EQ(vertex_cover[0], 1);
    EXPECT_EQ(vertex_cover[1], 0);
    EXPECT_EQ(vertex_cover[2], 3);
    EXPECT_EQ(vertex_cover[3], 2);
    EXPECT_EQ(vertex_cover[4], 4);
}

TEST(GeneratorTest, Grid) {
    graph g = generateGridGraph(2, 2);

    EXPECT_EQ(g.isConnected(0, 1), GRAPH_TRUE);
    EXPECT_EQ(g.isConnected(0, 2), GRAPH_TRUE);
    EXPECT_EQ(g.isConnected(1, 3), GRAPH_TRUE);
    EXPECT_EQ(g.isConnected(2, 3), GRAPH_TRUE);
}

TEST(GeneratorTest, Hypercube) {
    graph g = generateHypercubeGraph(4);

    EXPECT_EQ(g.isConnected(0, 1), GRAPH_TRUE);
    EXPECT_EQ(g.isConnected(0, 2), GRAPH_TRUE);
    EXPECT_EQ(g.isConnected(1, 3), GRAPH_TRUE);
    EXPECT_EQ(g.isConnected(2, 3), GRAPH_TRUE);
}

TEST(GeneratorTest, generateCompBipartiteGraph) {
    graph g = generateCompBipartiteGraph(2);

    EXPECT_EQ(g.isConnected(0, 2), GRAPH_TRUE);
    EXPECT_EQ(!g.isConnected(0, 1), GRAPH_TRUE);
    EXPECT_EQ(g.isConnected(0, 3), GRAPH_TRUE);
    EXPECT_EQ(g.isConnected(1, 2), GRAPH_TRUE);
    EXPECT_EQ(!g.isConnected(2, 3), GRAPH_TRUE);
    EXPECT_EQ(g.isConnected(1, 3), GRAPH_TRUE);
}

//TEST(GeneratorTest, Butterfly) {
//    graph g = generateButterflyGraph(3);
//
//    EXPECT_EQ(1, 1);
//}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
