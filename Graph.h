//
// Created by Dan Schwartz on 8/3/20.
//

#ifndef MTM_FINAL_GRAPH_H
#define MTM_FINAL_GRAPH_H

#include "VertexName.h"

#include <iostream>
#include <set>
#include <utility>
#include <algorithm>

class Graph{
    std::set<VertexName> vertices;
    std::set<std::pair<VertexName, VertexName> > edges;

    static Graph fullGraph(std::set<VertexName> vertices);

public:
    Graph() = default;
    Graph(std::set<VertexName> &vertices_parameter, std::set<std::pair<VertexName,VertexName> > &edges_parameter);
    Graph(std::multiset<VertexName> &vertices_parameter, std::multiset<std::pair<VertexName,VertexName> > &edges_parameter);
    //~Graph() = default;
    Graph(Graph const &graph) = default;
    Graph& operator=(Graph const &graph) = default;

    Graph operator+(Graph const &graph);
    Graph operator^(Graph const &graph);
    Graph operator-(Graph const &graph);
    Graph operator*(Graph const &graph);
    Graph operator!();

    std::set<VertexName> getVertices() const;
    std::set<std::pair<VertexName, VertexName> > getEdges() const;

    void setNewVertex(char* &vertex_name);
    void setNewEdge(char* &src_name, char* &dst_name);

    friend std::ostream &operator<<(std::ostream &os, const Graph &graph);

    class EdgesHaveVerticesNotInGraph : public std::exception {
        std::basic_string<char> return_message;
    public:
        explicit EdgesHaveVerticesNotInGraph(const VertexName &vertex, const std::pair<VertexName,VertexName> &edge);
        //~EdgesHaveVerticesNotInGraph() = default;
        const char* what() const noexcept override;

    };

    class DuplicateVertices : public std::exception {
        std::basic_string<char> return_message;
    public:
        explicit DuplicateVertices(const VertexName &vertex);
        //~EdgesHaveVerticesNotInGraph() = default;
        const char* what() const noexcept override;

    };

    class ParallelEdges : public std::exception {
        std::basic_string<char> return_message;
    public:
        explicit ParallelEdges(const std::pair<VertexName,VertexName> &edge);
        //~EdgesHaveVerticesNotInGraph() = default;
        const char* what() const noexcept override;

    };

};



#endif //MTM_FINAL_GRAPH_H
