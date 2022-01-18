//
// Created by emi on 02/12/2021.
//
#include <algorithm>
#include <random>
#include "../include/graph.h"
#include "../include/randomGraph.h"

namespace galplib{
    std::vector<std::vector<unsigned int>> simpleTriangle(){
        std::vector<std::vector<unsigned int>> testData = std::vector<std::vector<unsigned int>>();
        testData.push_back({2, 3, 3});
        testData.push_back({1, 2, 1});
        testData.push_back({1, 3, 5});
        return testData;
    }

    std::vector<std::vector<unsigned int>> simpleConnComp(){
        std::vector<std::vector<unsigned int>> testData = std::vector<std::vector<unsigned int>>();
        testData.push_back({1, 5, 3});
        testData.push_back({2, 4, 1});
        testData.push_back({3, 4, 5});
        testData.push_back({4, 5, 5});
        testData.push_back({6, 7, 5});
        return testData;
    }

    std::vector<std::vector<unsigned int>> exampleGraph(){
        std::vector<std::vector<unsigned int>> testData = std::vector<std::vector<unsigned int>>();
        testData.push_back({1, 2, 3});
        testData.push_back({1, 4, 2});
        testData.push_back({1, 9, 4});
        testData.push_back({2, 8, 4});
        testData.push_back({3, 4, 6});
        testData.push_back({3, 6, 1});
        testData.push_back({3, 8, 2});
        testData.push_back({4, 5, 1});
        testData.push_back({5, 9, 8});
        testData.push_back({6, 7, 8});
        return testData;
    }


    void testExampleGraph(){
        auto testData = galplib::exampleGraph();
        galplib::Graph g = galplib::Graph(testData, 9);
        std::vector<unsigned int> prefs{7, 2, 8, 4, 5, 9, 3, 6, 2, 1};
        auto outp = g.computePath(prefs);
    }

    void testConnectedComponents(){
        auto testData = simpleConnComp();
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
    galplib::testExampleGraph();
//    galplib::testConnectedComponents();
    galplib::testErdosGeneration();
    return 0;
};