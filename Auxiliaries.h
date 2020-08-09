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
#include <cctype>
#include <fstream>
#include <queue>
#include <stack>
#include <list>
#include <map>

#include "Graph.h"

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string& s);
std::string rtrim(const std::string& s);
std::string trim(const std::string& s);

std::basic_string<char> eliminate_spaces(std::basic_string<char> const &string);

void checkValidGraphName(std::basic_string<char> &graph_name);

std::queue<std::basic_string<char> > reversePolishNotation(std::vector<std::string> const & command);


std::vector<std::string> convertCommandToVector(std::string command);
bool containsGraphLiteral(std::string const &command);
std::string findGraphLiteral(std::string const &command);
bool containsLoad(std::string const &command);
std::string findLoad(std::string const &command);
bool containsKeyWord(std::vector<std::string> const &command_vector);
std::string findKeyWord(std::vector<std::string> const &command_vector);
std::string findFileInLoad(std::string const &load_command);

void saveGraph(Graph const & graph, std::string const & filename);
Graph loadGraph(std::string const & filename);

class UnrecognizedCommand : public std::exception{
    std::basic_string<char> return_message;
public:
    //~UnrecognizedCommand() = default;
    const char* what() const noexcept override;
    explicit UnrecognizedCommand(const std::basic_string<char>& command);
};

class UndefinedVariable : public std:: exception{
    std::basic_string<char> return_message;
public:
    //~UndefinedVariable() = default;
    const char* what() const noexcept override;
    explicit UndefinedVariable(const std::basic_string<char>& variable);
};

class InvalidGraphName : public std:: exception{
    std::basic_string<char> return_message;
public:
    //~InvalidGraphName() = default;
    const char* what() const noexcept override;
    explicit InvalidGraphName(const std::basic_string<char>& graph_name);
};

class UnableToLoadGraph : public std:: exception{
    std::basic_string<char> return_message;
public:
    //~InvalidGraphName() = default;
    const char* what() const noexcept override;
    explicit UnableToLoadGraph(const std::basic_string<char>& graph_name);
};

class UnableToSaveGraph : public std:: exception{
    std::basic_string<char> return_message;
public:
    //~InvalidGraphName() = default;
    const char* what() const noexcept override;
    explicit UnableToSaveGraph(const std::basic_string<char>& file_name);
};

class UnableToOpenFile : public std:: exception{
    std::basic_string<char> return_message;
public:
    //~InvalidGraphName() = default;
    const char* what() const noexcept override;
    explicit UnableToOpenFile(const std::basic_string<char>& file_name);
};

#endif //MTM_FINAL_AUXILIARIES_H
