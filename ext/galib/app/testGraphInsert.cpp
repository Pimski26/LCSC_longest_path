//
// Created by emi on 02/12/2021.
//
#include <algorithm>
#include <random>
#include "../include/graph.h"
#include "../include/randomGraph.h"
#include "../include/testGraphs.h"

namespace graph_lib{
    // Code test below
    void testExampleGraph(){
        auto testData = graph_lib::exampleGraph(9);
        auto g = graph_lib::Graph(testData, 9);
        std::vector<unsigned int> prefs{7, 2, 8, 4, 5, 9, 3, 6, 2, 1};
        auto outp = g.computePath(prefs);
        auto gen = std::mt19937(0);
        auto o1 = g.getRandomEdge(1, gen);
        auto o2 = g.getRandomEdge(1, gen);
        auto o3 = g.getRandomEdge(1, gen);
        ;
    }

    void testConnectedComponents(){
        auto testData = simpleConnComp(7);
        findConnectedComponents(testData, 7);
    }

    void testErdosGeneration(){
        auto g1 = erdosGraphRecursive(10, 0.3, 0);
        1 + 1;
        auto g2 = erdosGraphRejection(10, 0.3, 0);
        2 + 2;
    }
}

int main() {
    graph_lib::testExampleGraph();
    graph_lib::testConnectedComponents();
    graph_lib::testErdosGeneration();
    return 0;
};