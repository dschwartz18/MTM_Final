//
// Created by Dan Schwartz on 8/5/20.
//

#ifndef MTM_FINAL_AUXILIARIES_H
#define MTM_FINAL_AUXILIARIES_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <sstream>

#include "Graph.h"
#include "VertexName.h"

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string& s);
std::string rtrim(const std::string& s);
std::string trim(const std::string& s);

std::basic_string<char> eliminate_spaces(std::basic_string<char> const &string);

class UnrecognizedCommand : public std::exception{
    std::basic_string<char> return_message;
public:
    ~UnrecognizedCommand() = default;
    const char* what() const noexcept override;
    explicit UnrecognizedCommand(const std::basic_string<char>& command);
};

class UndefinedVariable : public std:: exception{
    std::basic_string<char> return_message;
public:
    ~UndefinedVariable() = default;
    const char* what() const noexcept override;
    explicit UndefinedVariable(const std::basic_string<char>& variable);
};


#endif //MTM_FINAL_AUXILIARIES_H
