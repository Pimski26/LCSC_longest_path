//
// Created by emi on 09/12/2021.
//
#include <queue>
#include <list>
#include <tgmath.h>
#include "../include/randomGraph.h"

namespace galplib{
    Graph erdosGraphRecursive(unsigned int nodes, double p, int seed) {
        srand(seed);
        auto input = erdosInputGen(nodes, p, seed);
        auto concomp = findConnectedComponents(input, nodes);
        while (concomp.size() > 1) {
            //make a new Erdos graph where each node represents one of the connected components. (name c-graph)
            auto input2 = erdosInputGen(concomp.size(), p, rand());
            for (auto entry : input2) {
                std::vector<unsigned int> addendum; // Turn edges from c-graph edges into edges in our real graph.
                // entry = {i', j', x}, i' and j'  are the connected components number, or nodes in the c-graph
                std::sample(concomp[entry[0]].begin(), concomp[entry[0]].end(), std::back_inserter(addendum), 1, rand); // Select i
                std::sample(concomp[entry[1]].begin(), concomp[entry[1]].end(), std::back_inserter(addendum), 1, rand); // Select j
                addendum.push_back((rand() % (nodes*nodes))+1); // Select w.
                input.push_back(addendum); // Add the new edge into our main input.
            }
            sortInput(input); // Sort because we require sorted list of edges for memory segmentation reasons.
            concomp = findConnectedComponents(input, nodes);
        }
        return Graph(input, nodes);
    }

    template<typename T>
    bool sizeCompare(std::vector<T> a, std::vector<T> b){
        return a.size() < b.size();
    }

    Graph erdosGraphRejection(unsigned int nodes, double p, int seed) {
        srand(seed);
        while (true){
            unsigned int genSize = 1 * nodes * sqrt(nodes); //tune later?
            auto input = erdosInputGen(genSize, p, rand());
            auto concomp = findConnectedComponents(input, nodes);
            auto largestcomp = *std::max_element(concomp.begin(), concomp.end(), sizeCompare<unsigned int>);
            if (largestcomp.size() == nodes){
                std::unordered_map<unsigned int, unsigned int> indexFixer = std::unordered_map<unsigned int, unsigned int>();
                for (int i = 1; i <= nodes; i++)
                {
                    indexFixer.emplace(largestcomp[i-1], i);
                }
                std::vector<std::vector<unsigned int>> newInput;
                for (auto entry : input){
                    if (indexFixer.contains(entry[0]) && indexFixer.contains(entry[1])){
                        std::vector<unsigned int> newEdge = {indexFixer[entry[0]], indexFixer[entry[1]], entry[2]};
                        newInput.push_back(newEdge);
                    }
                }
                return Graph(input, nodes);
            }
        }
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