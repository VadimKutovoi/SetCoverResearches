// Copyright 2020 Vadim Kutovoi

#include "../src/graph/graph.h"
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
    g.addEdge(0, 1);
    EXPECT_TRUE(g.isConnected(0, 1));
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

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
