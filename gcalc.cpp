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
        if (symbol_table.count("_graph") > 0) {
            symbol_table.erase("_graph");

        }
        throw(UndefinedVariable(graph_variable));
    }
}

Graph calculatePolishNotation(std::queue<std::string> &rpolish_queue, std::string &command){
    Graph calculated_graph;
    std::string calculated_graph_name = "_graph";
    symbol_table[calculated_graph_name] = calculated_graph;
    std::stack<std::string> calculation_stack;

    while(!rpolish_queue.empty()){
        if(isalpha(rpolish_queue.front()[0])){
            calculation_stack.push(rpolish_queue.front());
            rpolish_queue.pop();
        } else if(rpolish_queue.front()[0] == '!') {
            checkGraphVariable(calculation_stack.top());
            symbol_table[calculated_graph_name] = !(symbol_table[calculation_stack.top()]);
            calculation_stack.pop();
            calculation_stack.push(calculated_graph_name);
            rpolish_queue.pop();
        } else {
            checkGraphVariable(calculation_stack.top());
            Graph right_side = symbol_table[calculation_stack.top()];
            calculation_stack.pop();

            checkGraphVariable(calculation_stack.top());
            Graph left_side = symbol_table[calculation_stack.top()];
            calculation_stack.pop();

            switch(rpolish_queue.front()[0]){
                case '+': symbol_table[calculated_graph_name] = left_side + right_side; break;
                case '^': symbol_table[calculated_graph_name] = left_side ^ right_side; break;
                case '*': symbol_table[calculated_graph_name] = left_side * right_side; break;
                case '-': symbol_table[calculated_graph_name] = left_side - right_side; break;
                default: throw(UnrecognizedCommand(command));
            }

            calculation_stack.push(calculated_graph_name);
            rpolish_queue.pop();
        }
    }
    Graph return_graph = symbol_table[calculated_graph_name];
    symbol_table.erase(calculated_graph_name);

    return return_graph;
}


Graph calculate(std::string &command){
    std::queue<basic_string<char> > command_reverse_polish_notation = reversePolishNotation(command);
    return calculatePolishNotation(command_reverse_polish_notation, command);
}


Graph create_graph(basic_string<char> const &command) {
    std::set<VertexName> vertices;
    std::set<std::pair<VertexName, VertexName> > edges;

    int start = command.find('{');
    int end = command.find('}');
    basic_string<char> new_command = command.substr(start + 1, end - start - 1);


    basic_string<char> vertices_string;
    basic_string<char> edges_string;

    int graph_separator = command.find('|');
    if (graph_separator == int(std::string::npos)) {
        vertices_string = new_command;
    } else {
        vertices_string = new_command.substr(0, graph_separator - 1);
        edges_string = trim(new_command.substr(graph_separator));

        while (edges_string.length() != 0) {
            start = edges_string.find('<');
            end = edges_string.find('>');

            basic_string<char> edge = edges_string.substr(start + 1, end - start - 1);
            int first_comma = edges_string.find(',');
            if (first_comma < start && first_comma != -1) {
                start = first_comma;
            }
            edges_string.erase(start, end - start + 1);
            edges_string = trim(edges_string);

            edge = trim(edge);
            int edge_separator = edge.find(',');
            if (edge_separator == int(std::string::npos)) {
                throw (UnrecognizedCommand(command));
            }
            VertexName src_vertex(trim(edge.substr(0, edge_separator)));
            VertexName dest_vertex(trim(edge.substr(edge_separator + 1)));
            edges.insert(std::pair<VertexName, VertexName>(src_vertex, dest_vertex));
        }
    }
        std::stringstream vertices_stream(vertices_string);
        while (vertices_stream.good()) {
            basic_string<char> vertex;
            getline(vertices_stream, vertex, ',');
            vertex = trim(vertex);
            VertexName vertex_name(vertex);
            vertices.insert(vertex_name);
        }

        Graph new_graph(vertices, edges);
        return new_graph;
}


void mathematicalCommand(std::basic_string<char> const &command){
    Graph calculated_graph;

    int equal_appearance = command.find('=');
    basic_string<char> assignee_graph_name = command.substr(0, equal_appearance);
    assignee_graph_name = trim(assignee_graph_name);
    checkValidGraphName(assignee_graph_name);

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



void word_command(std::basic_string<char> const &command, std::ostream &stream){
    std::vector<std::basic_string<char> > word_commands{"delete", "print", "reset", "who", "save", "load"};
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
                stream << graph_pair.first << std::endl;
            }
            break;
        case reset:
            symbol_table.clear();
            break;
        case print:
            stream << symbol_table[command_on];
        case save:
            //TODO
            break;
        case load:
            break;
        default:
            break;
    }
}


void read_command(std::basic_string<char> const &command, std::ostream &os){
    if(command.find('=') != std::string::npos){
        mathematicalCommand(command);
    } else {
        word_command(command, os);
    }
}

int main(int argc, char** argv){

    if(argc == 1){
        std::basic_string<char> command;
        while("quit" != trim(command)) {
            cout << "Gcalc> ";
            getline(cin, command);

            try {
                read_command(command, cout);
            } catch (UnrecognizedCommand &error) {
                cout << error.what() << std::endl;
                continue;
            } catch (UndefinedVariable &error) {
                cout << error.what() << std::endl;
                continue;
            } catch (InvalidGraphName &error) {
                cout << error.what() << std::endl;
                continue;
            } catch (VertexName::InvalidVertexName &error) {
                cout << error.what() << std::endl;
                continue;
            } catch(Graph::EdgesHaveVerticesNotInGraph &error){
                cout << error.what() << std::endl;
                continue;
            }
        }
    } else if(argc == 3){

        std::ifstream input_file;
        input_file.open(argv[1]);

        std::ofstream output_file;
        output_file.open(argv[2]);
//add check if files open correctly

        std::basic_string<char> command;
        while(input_file.good() && "quit" != trim(command)){
            getline(input_file, command);

            try {
                read_command(command, output_file);
            } catch (UnrecognizedCommand &error) {
                output_file << error.what() << std::endl;
                continue;
            } catch (UndefinedVariable &error) {
                output_file << error.what() << std::endl;
                continue;
            } catch (InvalidGraphName &error) {
                output_file << error.what() << std::endl;
                continue;
            } catch (VertexName::InvalidVertexName &error) {
                output_file << error.what() << std::endl;
                continue;
            } catch(Graph::EdgesHaveVerticesNotInGraph &error){
                output_file << error.what() << std::endl;
                continue;
            }

        }
    } else {
        std::cerr << "Wrong amount of arguments";
    }
}