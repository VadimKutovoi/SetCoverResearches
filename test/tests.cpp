// Copyright 2020 Vadim Kutovoi

#include "../src/graph/graph.h"
#include <gtest/gtest.h>

TEST(GraphTest, CanAddEdge) {
    graph g(2);
    EXPECT_NO_THROW(g.addEdge(0, 1));
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

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
