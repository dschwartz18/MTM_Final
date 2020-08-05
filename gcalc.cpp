//
// Created by Dan Schwartz on 8/3/20.
//


#include "Auxiliaries.h"

using std::cout;
using std::cin;
using std::basic_string;



//global symbol table
std::map<std::basic_string<char>, Graph> symbol_table;


void checkGraphVariable(basic_string<char> const &graph_variable){
    if (symbol_table.count(graph_variable) != 1){
        throw(UndefinedVariable(graph_variable));
    }
}

Graph calculate(std::basic_string<char> &command) {
    char operation = 0;
    for(auto const& character : command){
        if(ispunct(character)){
            operation = character;
            break;
        }
    }

    if(operation == '!'){
        basic_string<char> graph_variable = trim(command.substr(0, command.find(operation) - 1));
        if((command.substr(command.find(operation)).length() > 1)){
            throw(UnrecognizedCommand(command));
        }
        checkGraphVariable(graph_variable);
        return !(symbol_table[graph_variable]);
    } else{
        basic_string<char> graph_variable_left = trim(command.substr(0, command.find(operation) - 1));
        basic_string<char> graph_variable_right = trim(command.substr(command.find(operation) + 1));
        checkGraphVariable(graph_variable_left);
        checkGraphVariable(graph_variable_right);

        switch(operation){
            case '+': symbol_table[graph_variable_left] + symbol_table[graph_variable_right];
            case '^': symbol_table[graph_variable_left] ^ symbol_table[graph_variable_right];
            case '-': symbol_table[graph_variable_left] - symbol_table[graph_variable_right];
            case '*': symbol_table[graph_variable_left] * symbol_table[graph_variable_right];
            default: throw(UnrecognizedCommand(command));
        }
    }
}


Graph create_graph(basic_string<char> const &command) {
    std::set<VertexName> vertices;
    std::set<std::pair<VertexName, VertexName>> edges;

    int start = command.find('{');
    int end = command.find('}');
    basic_string<char> new_command = command.substr(start+1, end-start-1);


    basic_string<char> vertices_string;
    basic_string<char> edges_string;

    int graph_separator = command.find('|');
    if(graph_separator == std::string::npos){
        vertices_string = new_command;
    } else {
        vertices_string = new_command.substr(0, graph_separator - 1);
        edges_string =  trim(new_command.substr(graph_separator));

        while(edges_string.length() != 0){
            start = edges_string.find('<');
            end = edges_string.find('>');

            basic_string<char> edge = edges_string.substr(start + 1, end - start - 1);
            int first_comma = edges_string.find(',');
            if(first_comma < start && first_comma != -1){
                start = first_comma;
            }
            edges_string.erase(start, end - start + 1);
            edges_string = trim(edges_string);

            edge = trim(edge);
            int edge_separator = edge.find(',');
            if(edge_separator == std::string::npos){
                throw(UnrecognizedCommand(command));
            }
            try{
                VertexName src_vertex(trim(edge.substr(0, edge_separator)));
                VertexName dest_vertex(trim(edge.substr(edge_separator+1)));
                edges.insert(std::pair<VertexName,VertexName>(src_vertex, dest_vertex));
            } catch(VertexName::InvalidVertexName& name){
                name.what();
            }
        }
    }

    std::stringstream vertices_stream(vertices_string);
    while(vertices_stream.good()) {
        basic_string<char> vertex;
        getline(vertices_stream, vertex, ',');
        try{
            vertex = trim(vertex);
            VertexName vertex_name(vertex);
            vertices.insert(vertex_name);
        } catch(VertexName::InvalidVertexName& name){
            name.what();
        }
    }

    try {
        Graph new_graph(vertices, edges);
        return new_graph;
    } catch(Graph::EdgesHaveVerticesNotInGraph &e){
        e.what();
    }
}

void mathematicalCommand(std::basic_string<char> const &command){
    Graph calculated_graph;

    int equal_appearance = command.find('=');
    basic_string<char> assignee_graph_name = command.substr(0, equal_appearance);
    assignee_graph_name = trim(assignee_graph_name);

    basic_string<char> command_to_calculate = command.substr(equal_appearance + 1, command.size() - equal_appearance);
    command_to_calculate = trim(command_to_calculate);

    int matching_parentheses = 0;

    if(command_to_calculate.find('{') != std::string::npos){
        matching_parentheses++;
        if(command_to_calculate.find('}') != std::string::npos){
            matching_parentheses++;
            if((command_to_calculate.find('}') - command_to_calculate.find('{') + 1) == command_to_calculate.length()){
                calculated_graph = create_graph(command_to_calculate);
            }
        }
    } else{
        calculated_graph = calculate(command_to_calculate);
    }
    if(matching_parentheses == 1){
        throw(UnrecognizedCommand(command));
    }
    symbol_table[assignee_graph_name] = calculated_graph;
}


void word_command(std::basic_string<char> const &command){
    std::vector<std::basic_string<char>> word_commands{"delete", "print", "reset", "who", "save", "load"};
    enum enumerated_commands{delete_enum, print, reset, who, save, load};
    int desired_command = 0;

    for(auto &word_command : word_commands){
        desired_command++;
        if(command.find(word_command) != std::string::npos){
            break;
        }
        if(desired_command == 7){
            throw(UnrecognizedCommand(command));
        }
    }
    /** who doesnt take any graph var*/

    auto command_number = (enumerated_commands)(desired_command-1);

    int command_on_start = command.find('(');
    int command_on_end = command.rfind(')');

    std::basic_string<char> command_on = command.substr(command_on_start + 1, (command_on_end-command_on_start) - 1);
    if(desired_command < 3){
        command_on = trim(command_on);
        checkGraphVariable(command_on);
    }
    switch(command_number){
        case delete_enum:
            symbol_table.erase(command_on);
            break;
        case who:
            for (auto const& graph_pair: symbol_table) {
                std::cout << graph_pair.first << std::endl;
            }
            break;
        case reset:
            symbol_table.clear();
            break;
        case print:
            std::cout << symbol_table[command_on];
        case save:
            //TODO
            break;
        case load:
            break;
        default:
            break;
    }
}


void read_command(std::basic_string<char> const &command){
    if(command.find('=') != std::string::npos){
        mathematicalCommand(command);
    } else {
        word_command(command);
    }
}

int main(int argc, char** argv){

    if(argc == 1){
        std::basic_string<char> command;
        while(true){
            cout << "Gcalc> ";
            getline(cin, command);
            if("quit" == trim(command)){
                break;
            }
            try{
                read_command(command);
            } catch(UnrecognizedCommand &error) {
                cout << error.what() << std::endl;
                continue;
            } catch(UndefinedVariable &error){
                cout << error.what() << std::endl;
                continue;
            }
        }
    } else if(argc == 3){
        //TODO go over reading a file
    } else {
        //error
    }
}