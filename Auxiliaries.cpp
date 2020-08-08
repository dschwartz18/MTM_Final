//
// Created by Dan Schwartz on 8/5/20.
//

#include "Auxiliaries.h"


std::string ltrim(const std::string& s){
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string& s){
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string& s){
    return rtrim(ltrim(s));
}


std::basic_string<char> eliminate_spaces(std::basic_string<char> const &string){
    std::basic_string<char> modified_string = string;
    modified_string.erase(std::remove_if(modified_string.begin(), modified_string.end(), ::isspace), modified_string.end());
    return modified_string;
}

void checkValidGraphName(std::basic_string<char> &graph_name){
    bool valid = true;
    for(auto const & character: graph_name){
        if(!(isalpha(character)) && !(isdigit(character))){
            valid = false;
            break;
        }
    }
    if(!isalpha(graph_name[0])){
        valid = false;
    }
    std::vector<std::basic_string<char> > word_commands{"delete", "print", "reset", "who", "save", "load"};
    for(auto const & key_word : word_commands){
        if(key_word == graph_name){
            valid = false;
        }
    }
    if(!valid){
        throw(InvalidGraphName(graph_name));
    }
}


bool containsGraphLiteral(std::string const &command){
    int openers = 0;
    int closers = 0;

    for(auto const & character: command){
        if(character == '{'){
            openers++;
        }
        if(character == '}'){
            closers++;
        }
        if(openers-closers > 1){
            throw(UnrecognizedCommand(command));
        }
        if(closers > openers){
            throw(UnrecognizedCommand(command));
        }
    }
    if(openers > closers){
        throw(UnrecognizedCommand(command));
    }
    return !(openers == 0 && closers == 0);
}

std::string findGraphLiteral(std::string const &command){
    int opener = command.find('{');
    int closer = command.find('}');

    std::string graph_literal = trim(command.substr(opener + 1, closer - opener - 1));

    return graph_literal;
}

bool containsLoad(std::string const &command){
    return command.find("load") != std::string::npos;
}

std::string findLoad(std::string const &command){
    int location = command.find("load");
    std::string temp = trim(command.substr(location));

    int end = temp.find(')');
    std::string load_command = trim(temp.substr(0, end + 1));

    return load_command;
}

std::string findFileInLoad(std::string const &load_command){
    int start = load_command.find('(');
    int end = load_command.find(')');
    std::string file_name = trim(load_command.substr(start + 1, end - start - 1));
    return file_name;
}

std::vector<std::string> convertCommandToVector(std::string command) {
    std::vector<std::string> tokens_list;

    while(command.length() > 0){
        int character_number;
        for(character_number = 0; character_number < int(command.length()); ++character_number) {
            if (ispunct(command[character_number])) {
                break;
            }
        }
        if(character_number > 0){
            tokens_list.push_back(trim(command.substr(0, character_number)));
        }
        if(command.substr(character_number, 1).length() > 0){
            tokens_list.push_back(command.substr(character_number, 1));
        }

        command.erase(0, character_number + 1);
        command = trim(command);
    }

    return tokens_list;
}

bool containsKeyWord(std::vector<std::string> const & command_vector){
    std::vector<std::basic_string<char> > word_commands{"delete", "print", "reset", "who", "save"};
    for(auto const & key_word : word_commands){
        if(!command_vector.empty()){
            if(key_word == command_vector[0]){
                return true;
            }
        }
    }
    return false;
}

std::string findKeyWord(std::vector<std::string> const &command_vector){
    std::vector<std::basic_string<char> > word_commands{"delete", "print", "reset", "who", "save"};
    for(auto const & key_word : word_commands){
        if(key_word == command_vector[0]){
            return key_word;
        }
    }
    //should not get here
    return "none";
}



std::queue<std::basic_string<char> > reversePolishNotation(std::vector<std::string> const & command){
    std::vector<std::string> tokens_vector = command;
    std::queue<std::string> output_queue;
    std::stack<std::string> operator_stack;

    while(!tokens_vector.empty()){
        std::string current_token = tokens_vector.front();
        tokens_vector.erase(tokens_vector.begin());

        //if its a graph
        if(isalpha(current_token[0])){
            output_queue.push(current_token);
        } else if(current_token[0] == '!' || current_token[0] == '('){
           operator_stack.push(current_token);
        } else if (current_token[0] != ')') {
            if(!operator_stack.empty()) {
                if ((operator_stack.top()[0] == '!' || !(operator_stack.top()[0] == '('))) {
                    output_queue.push(operator_stack.top());
                    operator_stack.pop();
                }
            }
            operator_stack.push(current_token);
        }  else if (current_token[0] == ')'){
            while(operator_stack.top()[0] != '('){
                output_queue.push(operator_stack.top());
                operator_stack.pop();
            }
            operator_stack.pop();
        }
    }
    while(!operator_stack.empty()){
        output_queue.push(operator_stack.top());
        operator_stack.pop();
    }
    return output_queue;
}

void saveGraph(Graph const & graph, std::string const & filename){
    std::set<VertexName> vertices = graph.getVertices();
    std::set<std::pair<VertexName, VertexName> > edges = graph.getEdges();

    std::ofstream outfile(filename, std::ios_base::binary);
    if(!outfile){
        throw(UnableToSaveGraph(filename));
    }

    unsigned int amount_of_vertices = vertices.size();
    unsigned int amount_of_edges = edges.size();

    outfile.write((const char*)&amount_of_vertices, sizeof(amount_of_vertices));
    outfile.write((const char*)&amount_of_edges, sizeof(amount_of_edges));

    for(auto const & vertex : vertices){
        if(!outfile.good()){
            throw(UnableToSaveGraph(filename));
        }
        unsigned int length_of_vertex_name = vertex.length();
        std::string vertex_name_string = vertex.toString();
        outfile.write((char*)&length_of_vertex_name, sizeof(length_of_vertex_name));
        outfile.write(&vertex_name_string[0], length_of_vertex_name);
    }

    for(auto const & edge: edges){
        if(!outfile.good()){
            throw(UnableToSaveGraph(filename));
        }

        std::string src_vertex = edge.first.toString();
        unsigned int src_vertex_length = src_vertex.length();

        std::string dst_vertex = edge.second.toString();
        unsigned int dst_vertex_length = src_vertex.length();

        outfile.write((char*)&src_vertex_length, sizeof(src_vertex_length));
        outfile.write(&src_vertex[0], src_vertex_length);
        outfile.write((char*)&dst_vertex_length, sizeof(dst_vertex_length));
        outfile.write(&dst_vertex[0], dst_vertex_length);
    }
    outfile.close();
}


Graph loadGraph(std::string const & filename){
    std::set<VertexName> vertices;
    std::set<std::pair<VertexName, VertexName> > edges;

    std::ifstream infile(filename, std::ios_base::binary);
    if(!infile){
        throw(UnableToLoadGraph(filename));
    }
    unsigned int amount_of_vertices;
    unsigned int amount_of_edges;

    infile.read((char*)&amount_of_vertices, sizeof(amount_of_vertices));
    infile.read((char*)&amount_of_edges, sizeof(amount_of_edges));

    for(unsigned int i = 0; i < amount_of_vertices; ++i){
        if(!infile.good()){
            throw(UnableToLoadGraph(filename));
        }
        unsigned int length_of_vertex_name;
        infile.read((char*)&length_of_vertex_name, sizeof(length_of_vertex_name));
        std::string vertex_name_string;
        vertex_name_string.resize(length_of_vertex_name);
        infile.read(&vertex_name_string[0], length_of_vertex_name);
        VertexName vertex_name(vertex_name_string);
        vertices.insert(vertex_name);
    }

    for(unsigned int i = 0; i < amount_of_edges; ++i){
        if(!infile.good()){
            throw(UnableToLoadGraph(filename));
        }

        unsigned int length_of_src_vertex_name;
        unsigned int length_of_dst_vertex_name;
        std::string src_vertex_name_string;
        std::string dst_vertex_name_string;


        infile.read((char*)&length_of_src_vertex_name, sizeof(length_of_src_vertex_name));
        src_vertex_name_string.resize(length_of_src_vertex_name);
        infile.read(&src_vertex_name_string[0], length_of_src_vertex_name);
        VertexName src_vertex(src_vertex_name_string);

        infile.read((char*)&length_of_dst_vertex_name, sizeof(length_of_dst_vertex_name));
        dst_vertex_name_string.resize(length_of_dst_vertex_name);
        infile.read(&dst_vertex_name_string[0], length_of_dst_vertex_name);
        VertexName dst_vertex(dst_vertex_name_string);
        edges.insert(std::pair<VertexName, VertexName>(src_vertex, dst_vertex));
    }

    infile.close();
    Graph graph(vertices, edges);
    return graph;
}



/**-------------------Error classes--------------------*/

const char *UnrecognizedCommand::what() const noexcept {
    return return_message.std::string::c_str();
}

UnrecognizedCommand::UnrecognizedCommand(const std::basic_string<char>& command) {
    return_message = "Error: Unrecognized command ";
    return_message +=  "'" + command + "'";
}

const char *UndefinedVariable::what() const noexcept {
    return return_message.std::string::c_str();
}

UndefinedVariable::UndefinedVariable(const std::basic_string<char> &variable) {
    return_message = "Error: Undefined variable ";
    return_message +=  "'" + variable + "'";
}

const char *InvalidGraphName::what() const noexcept {
    return return_message.std::string::c_str();
}

InvalidGraphName::InvalidGraphName(const std::basic_string<char> &graph_name) {
    return_message = "Error: Invalid graph name ";
    return_message +=  "'" + graph_name + "'";
}

const char *UnableToLoadGraph::what() const noexcept {
    return return_message.std::string::c_str();
}

UnableToLoadGraph::UnableToLoadGraph(const std::basic_string<char> &file_name) {
    return_message = "Error: Unable to load graph ";
    return_message +=  "'" + file_name + "'";
}

const char *UnableToSaveGraph::what() const noexcept {
    return return_message.std::string::c_str();
}

UnableToSaveGraph::UnableToSaveGraph(const std::basic_string<char> &file_name) {
    return_message = "Error: Unable to save graph to ";
    return_message +=  "'" + file_name + "'";
}

const char *UnableToOpenFile::what() const noexcept {
    return return_message.std::string::c_str();
}

UnableToOpenFile::UnableToOpenFile(const std::basic_string<char> &file_name) {
    return_message = "Error: Unable to open file ";
    return_message +=  "'" + file_name + "'";
}
