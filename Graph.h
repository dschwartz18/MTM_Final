//
// Created by Dan Schwartz on 8/3/20.
//

#ifndef MTM_FINAL_GRAPH_H
#define MTM_FINAL_GRAPH_H

#include "VertexName.h"

#include <iostream>
#include <set>
#include <map>
#include <utility>

class Graph{
    std::set<VertexName> vertices;
    std::set<std::pair<VertexName, VertexName>> edges;

    static Graph fullGraph(std::set<VertexName> vertices);

public:
    Graph() = default;
    Graph(std::set<VertexName> &vertices_parameter, std::set<std::pair<VertexName,VertexName>> &edges_parameter);
    ~Graph() = default;
    Graph(Graph const &graph) = default;
    Graph& operator=(Graph const &graph) = default;

    Graph operator+(Graph const &graph);
    Graph operator^(Graph const &graph);
    Graph operator-(Graph const &graph);
    Graph operator*(Graph const &graph);
    Graph operator!();

    friend std::ostream &operator<<(std::ostream &os, const Graph &graph);

    class EdgesHaveVerticesNotInGraph : public std::exception {};

};

#endif //MTM_FINAL_GRAPH_H