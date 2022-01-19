//
// Created by emi on 09/12/2021.
//
#include <list>
#include <algorithm>
#include <ctgmath>
#include <random>
#include "../include/randomGraph.h"

namespace graph_lib{
    Graph erdosGraphRecursive(unsigned int nodes, double p, int seed) {
        auto gen = std::mt19937(seed);
        auto input = erdosInputGen(nodes, p, seed);
        auto concomp = findConnectedComponents(input, nodes);
        while (concomp.size() > 1) {
            //make a new Erdos graph where each node represents one of the connected components. (name c-graph)
            auto input2 = erdosInputGen(concomp.size(), p, gen());
            for (auto entry : input2) {
                std::vector<unsigned int> addendum; // Turn edges from c-graph edges into edges in our real graph.
                // entry = {i', j', w}, i' and j'  are the connected components number, or nodes in the c-graph
                std::sample(concomp[entry[0]-1].begin(), concomp[entry[0]-1].end(), std::back_inserter(addendum), 1, gen); // Select i
                std::sample(concomp[entry[1]-1].begin(), concomp[entry[1]-1].end(), std::back_inserter(addendum), 1, gen); // Select j
                sort(addendum.begin(), addendum.end()); //Make sure i < j.
                addendum.push_back(entry[2]); // Select w.
                input.push_back(addendum); // Add the new edge into our main input.
            }
            sortInput(input); // Sort because we require sorted list of edges for memory segmentation reasons.
            concomp.clear();
            concomp = findConnectedComponents(input, nodes);
        }
        return Graph(input, nodes);
    }

    template<typename T>
    bool sizeCompare(std::vector<T> a, std::vector<T> b){
        return a.size() < b.size();
    }

    unsigned int determineGenSize(unsigned int nodes, double p){
        double np = nodes*p;
        if (np >= 1.1) return nodes;
        if (np > 0.9 && np < 1.1) return 1 * nodes * sqrt(nodes); //possibly tune to a better fitting constant.
        if (np <= 0.9) return std::max(1<<31, 1<<(nodes-1)); // This sounds really freaky, but the size will be log(this)...
    }

    Graph erdosGraphRejection(unsigned int nodes, double p, int seed) {
        srand(seed);
        unsigned int genSize = determineGenSize(nodes, p);
        while (true){
            auto input = erdosInputGen(genSize, p, rand());
            auto concomp = findConnectedComponents(input, nodes);
            auto largestcomp = *std::max_element(concomp.begin(), concomp.end(), sizeCompare<unsigned int>);
            // Found largest component of random graph of size n^3/2, if its correct size, output.
            if (largestcomp.size() == nodes){
                std::unordered_map<unsigned int, unsigned int> indexFixer = std::unordered_map<unsigned int, unsigned int>();
                for (int i = 1; i <= nodes; i++)
                {
                    indexFixer.emplace(largestcomp[i-1], i);
                } //indexFixer is a "index transform" from graph of size n^3/2 to the graph we want.
                std::vector<std::vector<unsigned int>> newInput;
                for (auto entry : input){ //For each edge in the n^3/2 graph, check if both i and j are in the index transform.
                    if (indexFixer.contains(entry[0]) && indexFixer.contains(entry[1])){
                        std::vector<unsigned int> newEdge = {indexFixer[entry[0]], indexFixer[entry[1]], entry[2]};
                        newInput.push_back(newEdge); //if it is contained, build a new edge like t(i), t(j), w.
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
            for (unsigned int yi = xi + 1; yi <= nodes; yi++) {
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
                if (visited[line[0]]){ // merge or ignore
                    auto iterFirst = output.begin(); // the connected component containing line[0]
                    while (iterFirst != output.end() && !listContains(*iterFirst, line[0])) {
                        iterFirst++;
                    }
                    if (iterFirst != iterSecond) { // If components area different, else do nothing.
                        for (auto node: *iterFirst) {
                            (*iterSecond).push_back(node);
                        }
                        output.remove(*iterFirst);
                    }
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
        auto vectOut = std::vector<std::vector<unsigned int>>(output.begin(), output.end());
        for (unsigned int i = 1; i <= nodes; i++){
            if (!visited[i]){
                vectOut.push_back({i});
            }
        }
        return vectOut;
    }
}