//
// Created by emi on 02/12/2021.
//
#include "../include/graph.h"


int main() {
    std::vector<std::vector<unsigned int>> testData = std::vector<std::vector<unsigned int>>();
    testData.push_back({2, 3, 3});
    testData.push_back({1, 2, 1});
    galplib::Graph g = galplib::Graph(testData, 3);
    int a = g.getEdge(1, 2);
    int b = g.getEdge(3, 2);
    int c = g.getEdge(3, 1);
    return 0;
};