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
    if(!valid){
        throw(InvalidGraphName(graph_name));
    }
}

//returns
std::queue<std::basic_string<char> > reversePolishNotation(std::basic_string<char> command){
    std::list<std::string> tokens_list;
    std::queue<std::string> output_queue;
    std::stack<std::string> operator_stack;

    //tokenize and put into a list
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

    while(!tokens_list.empty()){
        std::string current_token = tokens_list.front();
        tokens_list.pop_front();

        //if its a graph
        if(isalpha(current_token[0])){
            output_queue.push(current_token);
        } else if(current_token[0] == '!' || current_token[0] == '('){
           operator_stack.push(current_token);
        } else if (current_token[0] != ')'){
            if(!operator_stack.empty() && operator_stack.top()[0] == '!'){
                output_queue.push(operator_stack.top());
                operator_stack.pop();
            }
            operator_stack.push(current_token);
        } else if (current_token[0] == ')'){
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
