//
// Created by Dan Schwartz on 8/8/20.
//

#include "GraphPython.h"


Graph* create(){
    try{
        Graph* graph = new(Graph);
        return graph;
    } catch (std::bad_alloc& e){
        std::cout << "Error: Graph could not be created";
        return nullptr;
    }
}

void destroy(Graph *graph){
    delete(graph);
}

Graph* addVertex(Graph *graph, char* vertex){
    try {
        (*graph).setNewVertex(vertex);
        return graph;
    } catch (VertexName::InvalidVertexName& error){
        std::cout << error.what() << std::endl;
        return nullptr;
    } catch (Graph::DuplicateVertices& error){
        std::cout << error.what() << std::endl;
        return nullptr;
    }
}

Graph* addEdge(Graph *graph, char* src_vertex, char* dst_vertex){
    try {
        (*graph).setNewEdge(src_vertex, dst_vertex);
        return graph;
    } catch (VertexName::InvalidVertexName& error){
        std::cout << error.what() << std::endl;
        return nullptr;
    } catch (Graph::EdgesHaveVerticesNotInGraph& error){
        std::cout << error.what() << std::endl;
        return nullptr;
    } catch (Graph::ParallelEdges& error){
        std::cout << error.what() << std::endl;
        return nullptr;
    }
}

void disp(Graph *graph){
    std::cout << *graph;
}

Graph* graphUnion(Graph *graph_in1, Graph *graph_in2, Graph *graph_out){
    *graph_out = *graph_in1 + *graph_in2;
    return graph_out;
}

Graph* graphIntersection(Graph *graph_in1, Graph *graph_in2, Graph *graph_out){
    *graph_out = *graph_in1 ^ *graph_in2;
    return graph_out;
}

Graph* graphDifference(Graph *graph_in1, Graph *graph_in2, Graph *graph_out){
    *graph_out = *graph_in1 - *graph_in2;
    return graph_out;
}

Graph* graphProduct(Graph *graph_in1, Graph *graph_in2, Graph *graph_out){
    *graph_out = *graph_in1 * *graph_in2;
    return graph_out;
}

Graph* graphComplement(Graph *graph_in, Graph *graph_out){
    *graph_out = !(*graph_in);
    return graph_out;
}

