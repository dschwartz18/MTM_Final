//
// Created by Dan Schwartz on 8/3/20.
//

#include "VertexName.h"
#include <cctype>

VertexName::VertexName(std::basic_string<char> const &name){
    if(checkValid(name)){
        vertex_name = name;
    } else {
        throw InvalidVertexName(name);
    }
}

bool VertexName::checkValid(std::basic_string<char> const &name) {
    int parenthesesCounter = 0;
    int bracketCounter = 0;
    for(int character_number = 0; name[character_number] != '\0'; ++character_number){
        if(!std::isgraph(name[character_number])){
            return false;
        }
        switch(name[character_number]){
            case '(':
                parenthesesCounter++;
                break;
            case ')':
                parenthesesCounter--;
                break;
            case '[':
                bracketCounter++;
                break;
            case ']':
                bracketCounter--;
                break;
            case ';':
                if(bracketCounter == 0){
                    return false;
                }
                break;
            default:
                if(!(std::isalpha(name[character_number]) || std::isdigit(name[character_number]))){
                    return false;
                }
                break;
        }
        if(parenthesesCounter < 0 || bracketCounter < 0){
            return false;
        }
    }
    return !(parenthesesCounter != 0 || bracketCounter != 0);
}

std::ostream &operator<<(std::ostream &os, const VertexName &name){
    return os << name.vertex_name;
}


VertexName operator*(VertexName const &name1, VertexName const &name2) {
    std::basic_string<char> multiplied_name = '[' + name1.vertex_name + ';' + name2.vertex_name + ']';
    VertexName multiplied_vertex(multiplied_name);
    return multiplied_vertex;
}

bool VertexName::operator<(VertexName const &name) const{
    bool less_than = vertex_name < name.vertex_name;
    return less_than;
}

bool VertexName::operator!=(const VertexName &name) const {
    bool less_than = vertex_name != name.vertex_name;
    return less_than;
}

std::basic_string<char> VertexName::toString() const {
    return vertex_name;
}


VertexName::InvalidVertexName::InvalidVertexName(const std::basic_string<char> &vertex) {
    return_message = "Error: Invalid Vertex Name ";
    return_message +=  "'" + vertex + "'";
}

const char *VertexName::InvalidVertexName::what() const noexcept {
    return return_message.std::string::c_str();
}

