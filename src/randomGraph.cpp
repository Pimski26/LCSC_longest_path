//
// Created by emi on 09/12/2021.
//
#include "../include/randomGraph.h"

namespace galplib{
    Graph erdosGraphRecursive(unsigned int nodes, double p, int seed) {
        // generate
        // findConnectedComponents
        // if (connectedComponents.size() > 1)
        // erdosGraphRecursive(connectedComponents.size())
        // connect components
        // return
    }

    Graph erdosGraphRejection(unsigned int nodes, double p, int seed) {
        //while (notFoundOfSize){
        //    generate
        //    findConnectedComponents
        //    Sort
        //    last.size() = n
        //    foundSize
        //}
        // return foundGraph
    }

    std::vector<std::vector<unsigned int>> erdosInputGen(unsigned int nodes, double p, int seed) {
        std::vector<std::vector<unsigned int>> data = std::vector<std::vector<unsigned int>>();
        srand(seed);
        for (unsigned int xi = 1; xi < nodes; xi++) {
            for (unsigned int yi = yi + 1; yi < nodes; yi++) {
                auto r = static_cast<float> (rand() / static_cast<float> (RAND_MAX));
                if (r < p) {
                    auto w = rand() % (nodes*nodes);
                    data.push_back({xi, yi, w+1});
                }
            }
        }
        return data;
    }

    std::vector<std::vector<unsigned int>> findConnectedComponents(std::vector<std::vector<unsigned int>> input){

    }
}