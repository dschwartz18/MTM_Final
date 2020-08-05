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
