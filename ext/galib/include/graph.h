//
// Created by emi on 02/12/2021.
//
#ifndef LCSCGA_GRAPH_H
#define LCSCGA_GRAPH_H

#include <vector>
#include <unordered_map>
#include <random>

namespace graph_lib{
    void sortInput(std::vector<std::vector<unsigned int>> & input);

    class Graph{
    public:
        Graph(std::vector<std::vector<unsigned int>> & input, unsigned int nodes);
        std::vector<unsigned int> computePath(const std::vector<unsigned int> & prefs) const;
        int getEdge(unsigned int i, unsigned int j) const;
        void oneify();
        unsigned int getRandomEdge(unsigned int i, std::mt19937 & gen) const;
        std::unordered_map<unsigned int, int> getNodeEdgeSet(unsigned int i) const;
        unsigned int getNodeCount() const;
    private:
        std::vector<std::unordered_map<unsigned int, int>> edges;
        unsigned int nodeCount;
    };


}

#endif //LCSCGA_GRAPH_H
