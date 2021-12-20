//
// Created by emi on 09/12/2021.
//
#include <queue>
#include <list>
#include "../include/randomGraph.h"

namespace galplib{
    Graph erdosGraphRecursive(unsigned int nodes, double p, int seed) {
        srand(seed);
        auto input = erdosInputGen(nodes, p, seed);
        auto concomp = findConnectedComponents(input, seed);
        while (concomp.size() > 1) {
            auto input2 = erdosInputGen(concomp.size(), p, rand());
            std::vector<std::vector<unsigned int>> tbaI; //to be added i
            std::vector<std::vector<unsigned int>> tbaJ; //to be added j (edge (i,j))
            for (auto entry : input2) {
                std::sample(concomp[entry[0]].begin(), concomp[entry[0]].end(), std::back_inserter(tbaI), 1, rand);
                std::sample(concomp[entry[1]].begin(), concomp[entry[1]].end(), std::back_inserter(tbaJ), 1, rand);
            } // TODO generate w's and update concomp
        }// findConnectedComponents
        // if (concomp.size() > 1)
        // erdosGraphRecursive(concomp.size())
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
        for (unsigned int xi = 1; xi <= nodes; xi++) {
            for (unsigned int yi = yi + 1; yi <= nodes; yi++) {
                auto r = static_cast<float> (rand() / static_cast<float> (RAND_MAX));
                if (r < p) {
                    auto w = rand() % (nodes*nodes);
                    data.push_back({xi, yi, w+1});
                }
            }
        }
        return data;
    }

    bool listContains(std::vector<unsigned int> haystack, unsigned int needle){
        return std::find(haystack.begin(), haystack.end(), needle) != haystack.end();
    }

    std::vector<std::vector<unsigned int>> findConnectedComponents(std::vector<std::vector<unsigned int>> input, unsigned int nodes) {
        std::vector<bool> visited = std::vector<bool>(nodes+1);
        std::list<std::vector<unsigned int>> output;
        for (auto line : input) {
            if (visited[line[1]]) {
                auto iterSecond = output.begin(); // the connected component containing line[1].
                while (iterSecond != output.end() && !listContains(*iterSecond, line[1])) {
                    iterSecond++;
                }
                if (visited[line[0]]){ // merge
                    auto iterFirst = output.begin(); // the connected component containing line[0]
                    while (iterFirst != output.end() && !listContains(*iterFirst, line[0])) {
                        iterFirst++;
                    }
                    for (auto node : *iterFirst){
                        (*iterSecond).push_back(node);
                    }
                    output.remove(*iterFirst);
                }
                else{ // add
                    (*iterSecond).push_back(line[0]);
                    visited[line[0]] = true;
                }
            }
            else {
                auto iterSecond = output.begin(); // the connected component containing line[1]
                while (iterSecond != output.end() && !listContains(*iterSecond, line[0])) {
                    iterSecond++;
                }
                if (iterSecond == output.end()){ // new
                    std::vector<unsigned int> newComponent = std::vector<unsigned int>();
                    newComponent.reserve(nodes+1);
                    output.push_back(newComponent);
                    output.back().push_back(line[0]);
                    visited[line[0]] = true;
                    iterSecond = --output.end();
                } // add
                (*iterSecond).push_back(line[1]);
                visited[line[1]] = true;
            }
        }
    }
}