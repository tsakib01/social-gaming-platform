#ifndef EXPRESSION__H
#define EXPRESSION__H

#include <string_view>

class Expression {
public:
    virtual std::string_view getType() = 0;
    virtual ~Expression() = 0;
};

#endif