#ifndef EXPRESSION__H
#define EXPRESSION__H

#include <string>
#include <string_view>

class Expression {
public:
    virtual ~Expression() = default;
};

class NumberExpr : public Expression {
public:
    NumberExpr(int value) : m_value(value) {}
    int getValue() { return m_value; }
private:
    int m_value{};
};

class StringExpr : public Expression {
public:
    StringExpr(std::string_view value) : m_value(value) {}
    std::string_view getValue() { return m_value; }
private:
    std::string m_value{};
};

#endif