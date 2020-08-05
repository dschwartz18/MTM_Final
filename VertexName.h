//
// Created by Dan Schwartz on 8/3/20.friend std::ostream &operator<<(std::ostream &os, const VertexName &name)
//

#ifndef MTM_FINAL_VERTEXNAME_H
#define MTM_FINAL_VERTEXNAME_H

#include <string>

class VertexName{
    std::basic_string<char> vertex_name;
    static bool checkValid(const std::basic_string<char> &name);

public:
    explicit VertexName(std::basic_string<char> const &name);
    VertexName() = default;
    ~VertexName() = default;
    VertexName(VertexName const &name) = default;
    VertexName& operator=(VertexName const &name) = default;

    bool operator<(VertexName const &name) const;
    bool operator!=(VertexName const &name) const;

    friend VertexName operator*(VertexName const &name1, VertexName const &name2);
    friend std::ostream &operator<<(std::ostream &os, const VertexName &name);

    class InvalidVertexName : public std::exception {
        std::basic_string<char> name;

    public:
        explicit InvalidVertexName(const std::basic_string<char>& v_name);
        ~InvalidVertexName() = default;
    };

};

#endif //MTM_FINAL_VERTEXNAME_H
