//
// Created by emi on 02/12/2021.
//

#include "../include/graph.h"
#include <stdexcept>
#include <unordered_set>

namespace graph_lib {
    bool compareIndex(std::vector<unsigned int> I1, std::vector<unsigned int> I2){
        return I1[0] < I2[0];
    }

    void sortInput(std::vector<std::vector<unsigned int>> & input){
        std::sort(input.begin(), input.end(), compareIndex);
    }

    void inputFix (std::vector<std::vector<unsigned int>> & input){
        int initSize = input.size();
        for(auto i = 0; i< initSize; i++)
        {
            auto in = input[i];
            input.push_back({in[1], in[0], in[2]});
        }
        sortInput(input); // but actually e.g. iterators
    }

    Graph::Graph(std::vector<std::vector<unsigned int>> & input, unsigned int nodes) {
        inputFix(input);
        // make sure it's sorted!
        nodeCount = nodes;
        edges = std::vector<std::unordered_map<unsigned int, int>>();
        unsigned int iter = 0;
        // Note our graph nodes are 1-indexed. This means we need an empty edge set for node 0 to fill the datastructure.
        // However since index 0 should not be in the input, starting the loop at 0 will take care of that.
        for (int index = 0; index <= nodes; index++){
            // We make use of the fact that the input is sorted in the left vertex to be able to enforce that this does
            // not grow after we finish building it for this vertex to avoid segmentation faults.
            std::unordered_map<unsigned int, int> edgeMap = std::unordered_map<unsigned int, int>();
            while (iter < input.size() && input[iter][0] == index){
                // Add the next line of input to our graph by adding the edge.
                edgeMap.insert({input[iter][1], input[iter][2]});
                iter++;
            }
            edges.push_back(edgeMap);
        }
    }

    void Graph::oneify(){
        for (auto & edgeset : edges){
            for (auto & it : edgeset){
                it.second = 1;
            }
        }
    }

    // This function returns the weight of an edge, or -1 if it's not included in the graph. This assumes that
    // weights are >= 0.
    int Graph::getEdge(unsigned int i, unsigned int j) const {
        if (i > nodeCount || j > nodeCount){
            throw std::invalid_argument("node was not in graph!");
        }
        if (i < j){
            if (edges[i].contains(j)) return edges[i].at(j);
        } else {
            if (edges[j].contains(i)) return edges[j].at(i);
        }
        return -1;
    }

    std::unordered_map<unsigned int, int> Graph::getNodeEdgeSet(unsigned int i) const{
        return edges[i];
    }

    unsigned int Graph::getNodeCount() const {
        return nodeCount;
    }

    unsigned int Graph::getRandomEdge(unsigned int i, std::mt19937 & gen) const {
        auto r = getNodeEdgeSet(i);
        auto r0 = r;
        std::vector<std::pair<unsigned int, int>> returnable;
        std::sample(r.begin(), r.end(), std::back_inserter(returnable), 1, gen);
        return returnable[0].first;
    }

    //This function will return a path and it's length. The length is encoded in path[0], the vertices visited are
    // [1..n] where n is the total vertices in path.
    std::vector<unsigned int> Graph::computePath(const std::vector<unsigned int> & prefs) const{
        std::unordered_set<unsigned int> visited = std::unordered_set<unsigned int>(prefs.size());
        unsigned int current = prefs[0]; // Our nodes are all 1-indexed, meaning the 0th index is free to put as a start node.
        std::vector<unsigned int> path{0};
        while (true) {
            // We build the path starting at node, then going to prefs[node], e.g. hopping along the preferred edge at
            // each stage. This is valid so long that node has not been visited yet, hence the visited check.
            visited.insert(current);
            path.push_back(current);
            int w = getEdge(current, prefs[current]);
            if (w >= 0 && !visited.contains(prefs[current])){
                path[0] +=w;
                current = prefs[current];
                continue;   // If we successfully found a new node to add to the path (the if statement), do so
                            // and iterate the loop, which is what continue does.
            }
            break; // the default behaviour is to break, which feels slightly safer, ending the while loop.
        }
        return path;
    }

}
