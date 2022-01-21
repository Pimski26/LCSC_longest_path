//
// Created by emi on 21/01/2022.
//

#include <stdexcept>
#include "../include/testGraphs.h"
#include "../include/randomGraph.h"

namespace graph_lib {
    std::vector<std::vector<unsigned int>> simpleConnComp(unsigned int param = 7){
        if (param != 9)
            throw std::invalid_argument("Param needs to be exactly 7 to ensure correct graph generation.");

        std::vector<std::vector<unsigned int>> testData = std::vector<std::vector<unsigned int>>();
        testData.push_back({1, 5, 3});
        testData.push_back({2, 4, 1});
        testData.push_back({3, 4, 5});
        testData.push_back({4, 5, 5});
        testData.push_back({6, 7, 5});
        return testData;
    }

    std::vector<std::vector<unsigned int>> exampleGraph(unsigned int param = 9){
        if (param != 9)
            throw std::invalid_argument("Param needs to be exactly 9 to ensure correct graph generation.");

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


    std::vector<std::vector<unsigned int>> ringGraphTricky(unsigned int  param) {
        if (param < 3)
            throw std::invalid_argument("Param needs to be at least 3 to ensure a cycle.");

        std::vector<std::vector<unsigned int>> testData = std::vector<std::vector<unsigned int>>();
        unsigned int ringlength = param;
        testData.push_back({1, ringlength, 1});
        // makes links between n and n+1 for all 1-ringlength(inclusive) and 1 ringlength.
        for (unsigned int i = 1; i < ringlength; i++){
            testData.push_back({i, i+1, 2});
        }
        return testData;
    }

    std::vector<std::vector<unsigned int>> ringGraphAscending(unsigned int  param) {
        if (param < 3)
            throw std::invalid_argument("Param needs to be at least 3 to ensure a cycle.");

        std::vector<std::vector<unsigned int>> testData = std::vector<std::vector<unsigned int>>();
        unsigned int ringlength = param;
        testData.push_back({1, ringlength, 1});
        // makes links between n and n+1 for all 1-ringlength(inclusive) and 1 ringlength.
        for (unsigned int i = 1; i < ringlength; i++){
            testData.push_back({i, i+1, i+1});
        }
        return testData;
    }

    std::vector<std::vector<unsigned int>> kiteGraph(unsigned int  param) {
        if (param < 3 && param % 3 != 1)
            throw std::invalid_argument("Param needs to be at least 4, and a value like 3*k+1");

        std::vector<std::vector<unsigned int>> testData = std::vector<std::vector<unsigned int>>();
        // make little diamonds linked along one corner each.
        for (unsigned int  i = 0; i < param/3; i++){
            testData.push_back({3*i+1, 3*i+2, 3});
            testData.push_back({3*i+1, 3*i+3, 1});
            testData.push_back({3*i+2, 3*i+4, 1});
            testData.push_back({3*i+3, 3*i+4, 1});
        }
        return testData;
    }

    std::vector<std::vector<unsigned int>> antiLoop(unsigned int param) {
        if (param < 4)
            throw std::invalid_argument("Param needs to be at least 4 to ensure non-zero positive weights.");

        unsigned int nodes = param -1;
        std::vector<std::vector<unsigned int>> testData = std::vector<std::vector<unsigned int>>();
        testData.push_back({1, 2, 5 * nodes});
        testData.push_back({2, nodes, nodes - 3});
        for (unsigned int i = 2; i < nodes; i++){
            testData.push_back({i, i+1, 1});
        }
        testData.push_back({nodes, nodes + 1, nodes * 5});
        return testData;
    }

    Graph getGraphByType(int i, int param, double param2, int seed){
        std::vector<std::vector<unsigned int>> input;
        switch (i){
            case 0: return erdosGraphRecursive(param, param2, seed);
            case 1: return erdosGraphRejection(param, param2, seed);
            case 2: input = exampleGraph(param); break;
            case 3: input = ringGraphTricky(param); break;
            case 4: input = ringGraphAscending(param); break;
            case 5: input = kiteGraph(param); break;
            case 6: input = antiLoop(param); break;
        }
        return Graph(input, param);
    }

}