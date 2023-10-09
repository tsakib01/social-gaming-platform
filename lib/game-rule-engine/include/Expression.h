#ifndef EXPRESSION__H
#define EXPRESSION__H

#include <string_view>

class Expression {
public:
    virtual std::string_view getType() = 0;
    virtual ~Expression() = 0;
};

class NumberExpr : public Expression {
public:
    NumberExpr(int value) : value{value} {}
    int value{};
    std::string_view getType() { return "numberexpr"; }
    ~NumberExpr() = default;
};

#endif