#ifndef EXPRESSION__H
#define EXPRESSION__H

#include <string>
#include <string_view>
#include <vector>

class NumberExpr;
class StringExpr;
class ListExpr;

class Expression {
public:
    virtual ~Expression() = default;

    static std::unique_ptr<NumberExpr> 
    createNumber(int value) { return std::make_unique<NumberExpr>(value); }

    static std::unique_ptr<StringExpr>
    createString(std::string_view value) { return std::make_unique<StringExpr>(value); }
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