#ifndef EXPRESSION__H
#define EXPRESSION__H

#include <string>
#include <string_view>
#include <vector>

class IntExpression;
class StringExpression;

// Base class for all expressions
class Expression {
public:
    virtual ~Expression() = default;

    static std::unique_ptr<IntExpression> 
    createNumber(int value) { return std::make_unique<IntExpression>(value); }

    static std::unique_ptr<StringExpression>
    createString(std::string_view value) { return std::make_unique<StringExpression>(value); }
};


class IntExpression : public Expression {
public:
    IntExpression(int value) : m_value(value) {}
    int getValue() { return m_value; }
    
private:
    int m_value{};
};


class StringExpression : public Expression {
public:
    StringExpression(std::string_view value) : m_value(value) {}
    std::string_view getValue() { return m_value; }

private:
    std::string m_value{};
};





#endif