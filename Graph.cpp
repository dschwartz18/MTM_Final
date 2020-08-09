//
// Created by Dan Schwartz on 8/3/20.
//

#include "Graph.h"


Graph::Graph(std::set<VertexName> &vertices_parameter, std::set<std::pair<VertexName,VertexName> > &edges_parameter){
    for(auto const & edge: edges_parameter){
        if(vertices_parameter.find(edge.first) == vertices_parameter.end()){
            throw(EdgesHaveVerticesNotInGraph(edge.first, edge));
        }
        if(vertices_parameter.find(edge.second) == vertices_parameter.end()){
            throw(EdgesHaveVerticesNotInGraph(edge.second, edge));
        }
    }

    vertices = vertices_parameter;
    edges = edges_parameter;
}

Graph::Graph(std::multiset<VertexName> &vertices_parameter, std::multiset<std::pair<VertexName,VertexName> > &edges_parameter){

    for(auto const& vertex : vertices_parameter){
        if(vertices_parameter.count(vertex) > 1){
            throw(DuplicateVertices(vertex));
        }
    }
    for(auto const& edge : edges_parameter){
        if(edges_parameter.count(edge) > 1){
            throw(ParallelEdges(edge));
        }
    }

    std::set<VertexName> vertices_set(vertices_parameter.begin(), vertices_parameter.end());
    std::set<std::pair<VertexName,VertexName> > edges_set(edges_parameter.begin(), edges_parameter.end());

    for(auto const & edge: edges_set){
        if(vertices_set.find(edge.first) == vertices_set.end()){
            throw(EdgesHaveVerticesNotInGraph(edge.first, edge));
        }
        if(vertices_set.find(edge.second) == vertices_set.end()){
            throw(EdgesHaveVerticesNotInGraph(edge.second, edge));
        }
    }

    vertices = vertices_set;
    edges = edges_set;
}

Graph Graph::operator+(Graph const &graph) {
    std::set<VertexName> combined_vertices;
    std::set<std::pair<VertexName, VertexName> > combined_edges;

    std::set_union(vertices.begin(), vertices.end(), graph.vertices.begin(), graph.vertices.end(), std::inserter(combined_vertices, combined_vertices.begin()));
    std::set_union(edges.begin(), edges.end(), graph.edges.begin(), graph.edges.end(), std::inserter(combined_edges, combined_edges.begin()));

    Graph merged_graph(combined_vertices, combined_edges);

    return merged_graph;
}

Graph Graph::operator^(Graph const &graph) {
    std::set<VertexName> intersection_vertices;
    std::set<std::pair<VertexName, VertexName> > intersection_edges;

    std::set_intersection(vertices.begin(), vertices.end(), graph.vertices.begin(), graph.vertices.end(), std::inserter(intersection_vertices, intersection_vertices.begin()));
    std::set_intersection(edges.begin(), edges.end(), graph.edges.begin(), graph.edges.end(), std::inserter(intersection_edges, intersection_edges.begin()));

    Graph intersection_graph(intersection_vertices, intersection_edges);

    return intersection_graph;
}

Graph Graph::operator-(Graph const &graph) {
    std::set<VertexName> subtracted_vertices;
    std::set<std::pair<VertexName, VertexName> > subtracted_edges = edges;

    std::set_difference(vertices.begin(), vertices.end(), graph.vertices.begin(), graph.vertices.end(), std::inserter(subtracted_vertices, subtracted_vertices.begin()));

    for(auto edge = edges.begin(); edge != edges.end(); ++edge){
        if(subtracted_vertices.find(edge->first) == subtracted_vertices.end() || subtracted_vertices.find(edge->second) == subtracted_vertices.end()){
            edges.erase(edge);
        }
    }

    Graph subtracted_graph(subtracted_vertices, subtracted_edges);

    return subtracted_graph;
}

Graph Graph::operator*(Graph const &graph) {
    std::set<VertexName> multiplied_vertices;
    std::set<std::pair<VertexName, VertexName> > multiplied_edges;

    for(const auto & vertex1 : vertices){
        for(const auto & vertex2 : graph.vertices){
            VertexName multiplied_vertex = (vertex1*vertex2);
            multiplied_vertices.insert(multiplied_vertex);
        }
    }

    for(const auto & edge1 : edges){
        for(const auto & edge2 : graph.edges){
            multiplied_edges.insert(std::pair<VertexName,VertexName>((edge1.first*edge2.first),(edge1.second*edge2.second)));
        }
    }

    return Graph(multiplied_vertices, multiplied_edges);
}

Graph Graph::operator!() {
    std::set<VertexName> complement_vertices = vertices;
    std::set<std::pair<VertexName, VertexName> > complement_edges;
    Graph full_graph;
    full_graph = fullGraph(vertices);

    std::set_difference(full_graph.edges.begin(), full_graph.edges.end(), edges.begin(), edges.end(), std::inserter(complement_edges, complement_edges.begin()));

    Graph complement_graph(complement_vertices, complement_edges);
    return complement_graph;
}


Graph Graph::fullGraph(std::set<VertexName> vertices) {
    std::set<std::pair<VertexName, VertexName> > edges;

    for(auto src_vertex = vertices.begin(); src_vertex != vertices.end(); ++src_vertex){
        for(auto dest_vertex = vertices.begin(); dest_vertex != vertices.end(); ++dest_vertex){
            if(src_vertex != dest_vertex){
                edges.insert(std::pair<VertexName,VertexName>(*src_vertex, *dest_vertex));
            }
        }
    }

    Graph full_graph(vertices, edges);
    return full_graph;
}

std::set<VertexName> Graph::getVertices() const{
    return vertices;
}
std::set<std::pair<VertexName, VertexName> > Graph::getEdges() const{
    return edges;
}

std::ostream &operator<<(std::ostream &os, const Graph &graph) {
    for(auto const& vertex: graph.vertices){
        os << vertex << std::endl;
    }
    os << '$' << std::endl;
    for(auto const& edge: graph.edges){
        os << edge.first << ' ' << edge.second << std::endl;
    }
    return os;
}

void Graph::setNewEdge(char* &src_name, char* &dst_name) {
    VertexName src_vertex(src_name);
    VertexName dst_vertex(dst_name);

    std::pair<VertexName,VertexName> edge(src_vertex, dst_vertex);
    if(edges.find(edge) != edges.end()){
        throw(ParallelEdges(edge));
    }
    if(vertices.find(src_vertex) == vertices.end()){
        throw(EdgesHaveVerticesNotInGraph(src_vertex, edge));
    }
    if(vertices.find(edge.second) == vertices.end()){
        throw(EdgesHaveVerticesNotInGraph(dst_vertex, edge));
    }
    edges.insert(std::pair<VertexName,VertexName>(src_vertex, dst_vertex));

}

void Graph::setNewVertex(char* &name) {
    VertexName vertex_name(name);
    if(vertices.find(vertex_name) != vertices.end()){
        throw(DuplicateVertices(vertex_name));
    }
    vertices.insert(vertex_name);
}



Graph::EdgesHaveVerticesNotInGraph::EdgesHaveVerticesNotInGraph(const VertexName &vertex, const std::pair<VertexName,VertexName> &edge) {
    return_message = "Error: The edge ";
    return_message += "<" + edge.first.toString() + "," + edge.second.toString() + ">";
    return_message += " contains a vertex that is not in the graph ";
    return_message += "'" + vertex.toString() + "'";
}

const char *Graph::EdgesHaveVerticesNotInGraph::what() const noexcept {
    return return_message.std::string::c_str();
}

Graph::DuplicateVertices::DuplicateVertices(const VertexName &vertex) {
    return_message = "Error: A duplicated vertex was inputted ";
    return_message += "'" + vertex.toString() + "'";
}

const char *Graph::DuplicateVertices::what() const noexcept {
    return return_message.std::string::c_str();
}


Graph::ParallelEdges::ParallelEdges(const std::pair<VertexName,VertexName> &edge) {
    return_message = "Error: Parallel edges were inputted ";
    return_message += "<" + edge.first.toString() + "," + edge.second.toString() + ">";
}

const char *Graph::ParallelEdges::what() const noexcept {
    return return_message.std::string::c_str();
}
