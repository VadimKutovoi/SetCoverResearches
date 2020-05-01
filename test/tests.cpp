// Copyright 2020 Vadim Kutovoi

#include "../src/graph/graph.h"
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
    graph g(4);

    g.addEdge(0, 1);
    g.addEdge(0, 2);

    EXPECT_EQ(g.getAdjNum(0), 2);
    EXPECT_EQ(g.getAdjNum(1), 1);
    EXPECT_EQ(g.getAdjNum(2), 1);
    EXPECT_EQ(g.getAdjNum(3), 0);
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
    EXPECT_EQ(vertex_cover[2], 5);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
