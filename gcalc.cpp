//
// Created by Dan Schwartz on 8/3/20.
//

#include "Auxiliaries.h"

using std::cout;
using std::cin;
using std::basic_string;


//global symbol table
std::map<std::basic_string<char>, Graph> symbol_table;
std::string original_command;

void checkGraphVariable(basic_string<char> const &graph_variable){
    if (symbol_table.count(graph_variable) != 1){
        if (symbol_table.count("_graph") > 0) {
            symbol_table.erase("_graph");

        }
        throw(UndefinedVariable(graph_variable));
    }
}

Graph calculatePolishNotation(std::queue<std::string> &rpolish_queue){
    if(rpolish_queue.size() == 1){
        return symbol_table[rpolish_queue.front()];
    }

    Graph return_graph;
    Graph calculated_graph;
    Graph calculated_graph_complement;
    std::string calculated_graph_name = "_graph";
    std::string calculated_graph_name_complement = "!graph";
    symbol_table[calculated_graph_name] = calculated_graph;
    symbol_table[calculated_graph_name_complement] = calculated_graph_complement;
    std::stack<std::string> calculation_stack;


    while(!rpolish_queue.empty()){
        if(isalpha(rpolish_queue.front()[0])){
            calculation_stack.push(rpolish_queue.front());
            rpolish_queue.pop();
        } else if(rpolish_queue.front()[0] == '!') {
            checkGraphVariable(calculation_stack.top());
            symbol_table[calculated_graph_name_complement] = !(symbol_table[calculation_stack.top()]);
            calculation_stack.pop();
            calculation_stack.push(calculated_graph_name_complement);
            rpolish_queue.pop();
            return_graph = symbol_table[calculated_graph_name_complement];
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
                default: throw(UnrecognizedCommand(original_command));
            }
            calculation_stack.push(calculated_graph_name);
            rpolish_queue.pop();
            return_graph = symbol_table[calculated_graph_name];
        }
    }
    //Graph return_graph = symbol_table[calculated_graph_name];
    symbol_table.erase(calculated_graph_name);
    symbol_table.erase(calculated_graph_name_complement);

    return return_graph;
}


Graph calculate(std::vector<std::string> &command){
    std::queue<basic_string<char> > command_reverse_polish_notation = reversePolishNotation(command);
    return calculatePolishNotation(command_reverse_polish_notation);
}


Graph create_graph(basic_string<char> const &command) {
    std::set<VertexName> vertices;
    std::set<std::pair<VertexName, VertexName> > edges;

    basic_string<char> vertices_string;
    basic_string<char> edges_string;

    int graph_separator = command.find('|');
    if (graph_separator == int(std::string::npos)) {
        vertices_string = command;
    } else {
        vertices_string = command.substr(0, graph_separator);
        edges_string = trim(command.substr(graph_separator + 1));

        while (edges_string.length() != 0) {
            int start = edges_string.find('<');
            int end = edges_string.find('>');

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




std::string removeLiteralsAndLoad(std::basic_string<char> command){
    int counter = 0;
    std::string file_name;

    if(command.find("save") != std::string::npos){
        file_name = trim(command.substr(command.rfind(',')+1, command.rfind(')')-command.rfind(',')-1));
        command.replace(command.rfind(',') + 1, command.rfind(')') - command.rfind(',') - 1, "temp");
    }

    while(containsGraphLiteral(command)){
        counter++;
        std::string graph_literal = findGraphLiteral(command);
        Graph temp = create_graph(graph_literal);

        std::string const temp_graph_name = "TEMP" + std::to_string(counter);
        symbol_table[temp_graph_name] = temp;

        command.replace(command.find('{'), command.find('}')-command.find('{')+1, temp_graph_name);
    }

    while(containsLoad(command)){
        counter++;
        std::string load_command = findLoad(command);
        std::string file_to_load = findFileInLoad(load_command);
        Graph temp = loadGraph(file_to_load);

        std::string const temp_graph_name = "TEMP" + std::to_string(counter);
        symbol_table[temp_graph_name] = temp;

        command.replace(command.find("load"), load_command.length(), temp_graph_name);
    }
    if(command.find("save") != std::string::npos){
        command.replace(command.rfind(',') + 1, command.rfind(')') - command.rfind(',') - 1, file_name);
    }

    return command;
}

std::vector<std::string> checkParenthesesAndFindRest(std::vector<std::string> command_vector){
    std::string first = *(command_vector.begin());
    std::string last = *(command_vector.rbegin());

    if(first[0] != '(' || last[0] != ')'){
        throw(UnrecognizedCommand(original_command));
    } else {
        command_vector.erase(command_vector.begin());
        command_vector.pop_back();
    }

    return command_vector;
}

std::vector<std::string> findGraph(std::vector<std::string> command_vector){
    command_vector.erase(find(command_vector.begin(), command_vector.end(), ","), command_vector.end());
    return command_vector;
}

std::string findAndCheckFileName(std::vector<std::string> command_vector){
    if(find(command_vector.begin(), command_vector.end(), ",") == command_vector.end()){
        throw(UnrecognizedCommand(original_command));
    }

    command_vector.erase(command_vector.begin(), find(command_vector.begin(), command_vector.end(), ","));
    command_vector.erase(command_vector.begin());
    std::string file_name;

    for(auto const & token : command_vector){
        file_name += token;
    }

    for(auto const & character : file_name){
        if(character == '(' || character == ')' || character == ','){
            //TODO make new error invalid file name
            throw(UnrecognizedCommand(original_command));
        }
    }

    return file_name;
}

void wordCommand(std::vector<std::string> &command_vector, std::ostream &stream){
    std::string key_word = findKeyWord(command_vector);
    command_vector.erase(command_vector.begin());

    if(command_vector.empty()){
        if(key_word == "who"){
            for (auto const& graph_pair: symbol_table) {
                stream << graph_pair.first << std::endl;
            }
        } else if (key_word == "reset") {
            symbol_table.clear();
        } else {
            throw(UnrecognizedCommand(original_command));
        }
    } else {
        std::vector<std::string> withoutParentheses = checkParenthesesAndFindRest(command_vector);
        if(key_word == "save"){
            std::vector<std::string> graph_to_calc = findGraph(withoutParentheses);
            std::string file_name = findAndCheckFileName(withoutParentheses);

            saveGraph(calculate(graph_to_calc), file_name);

        } else if(key_word == "print"){
            stream << calculate(withoutParentheses);
        } else if(key_word == "delete"){
            if(withoutParentheses.size() != 1){
                throw(UnrecognizedCommand(original_command));
            } else {
                std::string graph_name = withoutParentheses[0];
                checkGraphVariable(graph_name);
                symbol_table.erase(graph_name);
            }
        }
    }
}


void clearTempGraphs(){
    std::vector<std::string> toDelete;
    for(auto const & graph : symbol_table) {
        if(graph.first.find("TEMP") == 0){
            toDelete.push_back(graph.first);
        }
    }
    for(auto const & temp_graph : toDelete){
        symbol_table.erase(temp_graph);
    }
}


void read_command(std::basic_string<char> &command, std::ostream &os){
    original_command = command;
    command = trim(command);
    command = removeLiteralsAndLoad(command);
    std::vector<std::string> command_vector = convertCommandToVector(command);

    if(containsKeyWord(command_vector)) {
        wordCommand(command_vector, os);
    } else if(command_vector.size() > 1  && command_vector[1][0] == '='){
        std::string graph_name = command_vector[0];
        command_vector.erase(command_vector.begin());
        command_vector.erase(command_vector.begin());

        Graph graph = calculate(command_vector);
        symbol_table[graph_name] = graph;
    } else {
        throw(UnrecognizedCommand(original_command));
    }
    clearTempGraphs();
}



int main(int argc, char** argv){

    if(argc == 1){
        std::basic_string<char> command;
        while("quit" != trim(command)) {
            cout << "Gcalc> ";
            getline(cin, command);
            if("quit" == trim(command)){
                break;
            }
            try {
                read_command(command, cout);
            } catch (UnrecognizedCommand &error) {
                clearTempGraphs();
                cout << error.what() << std::endl;
                continue;
            } catch (UndefinedVariable &error) {
                clearTempGraphs();
                cout << error.what() << std::endl;
                continue;
            } catch (InvalidGraphName &error) {
                clearTempGraphs();
                cout << error.what() << std::endl;
                continue;
            } catch (VertexName::InvalidVertexName &error) {
                clearTempGraphs();
                cout << error.what() << std::endl;
                continue;
            } catch(Graph::EdgesHaveVerticesNotInGraph &error){
                clearTempGraphs();
                cout << error.what() << std::endl;
                continue;
            } catch(UnableToLoadGraph &error){
                clearTempGraphs();
                cout << error.what() << std::endl;
                continue;
            } catch(UnableToSaveGraph &error){
                clearTempGraphs();
                cout << error.what() << std::endl;
                continue;
            }
        }
    } else if(argc == 3){

        try{
            std::ifstream input_file;
            input_file.open(argv[1]);
            if(!input_file){
                throw(UnableToOpenFile(argv[1]));
            }

            std::ofstream output_file;
            output_file.open(argv[2]);
            if(!output_file){
                throw(UnableToOpenFile(argv[2]));
            }


//add check if files open correctly

        std::basic_string<char> command;
        while(input_file.good() && "quit" != trim(command)){
            getline(input_file, command);
            if("quit" == trim(command)){
                break;
            }
            try {
                read_command(command, output_file);
            } catch (UnrecognizedCommand &error) {
                clearTempGraphs();
                output_file << error.what() << std::endl;
                continue;
            } catch (UndefinedVariable &error) {
                output_file << error.what() << std::endl;
                clearTempGraphs();
                continue;
            } catch (InvalidGraphName &error) {
                clearTempGraphs();
                output_file << error.what() << std::endl;
                continue;
            } catch (VertexName::InvalidVertexName &error) {
                clearTempGraphs();
                output_file << error.what() << std::endl;
                continue;
            } catch(Graph::EdgesHaveVerticesNotInGraph &error){
                clearTempGraphs();
                output_file << error.what() << std::endl;
                continue;
            } catch(UnableToLoadGraph &error){
                clearTempGraphs();
                output_file << error.what() << std::endl;
                continue;
            } catch(UnableToSaveGraph &error){
                clearTempGraphs();
                output_file << error.what() << std::endl;
                continue;
            }
        }

        } catch (UnableToOpenFile &error){
            cout << error.what() << std::endl;
        }
    } else {
        std::cerr << "Error: Wrong amount of arguments" << std::endl;
    }
    return 0;
}