//
// Created by Dan Schwartz on 8/8/20.
//

#include "GraphPython.h"


Graph create(){
    Graph graph;
    return graph;
}

void destroy(Graph graph){
    graph.~Graph();
}

Graph addVertex(Graph graph, char* vertex){
    graph.setNewVertex(vertex);
    return graph;
}

Graph addEdge(Graph graph, char* src_vertex, char* dst_vertex){
    graph.setNewEdge(src_vertex, dst_vertex);
    return graph;
}

void disp(Graph graph){
    std::cout << graph;
}

Graph graphUnion(Graph graph_in1, Graph graph_in2, Graph graph_out){
    graph_out = graph_in1 + graph_in2;
    return graph_out;
}

Graph graphIntersection(Graph graph_in1, Graph graph_in2, Graph graph_out){
    graph_out = graph_in1 ^ graph_in2;
    return graph_out;
}

Graph graphDifference(Graph graph_in1, Graph graph_in2, Graph graph_out){
    graph_out = graph_in1 - graph_in2;
    return graph_out;
}

Graph graphProduct(Graph graph_in1, Graph graph_in2, Graph graph_out){
    graph_out = graph_in1 * graph_in2;
    return graph_out;
}

Graph graphComplement(Graph graph_in, Graph graph_out){
    graph_out = !graph_in;
    return graph_out;
}

