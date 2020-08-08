//
// Created by Dan Schwartz on 8/8/20.
//

#ifndef MTM_FINAL_GRAPHPYTHON_H
#define MTM_FINAL_GRAPHPYTHON_H

#include "Graph.h"

Graph create();
void destroy(Graph graph);
Graph addVertex(Graph graph, char* vertex);
Graph addEdge(Graph graph, char* src_vertex, char* dst_vertex);
void disp(Graph graph);
Graph graphUnion(Graph graph_in1, Graph graph_in2, Graph graph_out);
Graph graphIntersection(Graph graph_in1, Graph graph_in2, Graph graph_out);
Graph graphDifference(Graph graph_in1, Graph graph_in2, Graph graph_out);
Graph graphProduct(Graph graph_in1, Graph graph_in2, Graph graph_out);
Graph graphComplement(Graph graph_in, Graph graph_out);

#endif //MTM_FINAL_GRAPHPYTHON_H
