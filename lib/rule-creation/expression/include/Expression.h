#ifndef EXPRESSION__H
#define EXPRESSION__H

#include "GameEnvironment.h"

class Expression;
class LiteralExpression;
class IdentifierExpression;
class BinaryExpression;
class UnaryExpression;
class QualifiedIdentifier;

class ExpressionVisitor {
public:
    virtual ~ExpressionVisitor() = default;
    virtual void visit(const LiteralExpression& expression) const = 0;
    virtual void visit(const IdentifierExpression& expression) const = 0;
    virtual void visit(const BinaryExpression& expression) const = 0;
    virtual void visit(const UnaryExpression& expression) const = 0;
    virtual void visit(const QualifiedIdentifier& expression) const = 0;
    virtual void visit(const Expression& expression) const = 0;
};

/// Operations that can be applied to expression(s).
enum class Operator{
    PLUS, SUBTRACT, MULTIPLY, DIVISION, OR, IDENTIFIER, DOT
};

/// An expression interface that represents a statement in the game config that can be evaluated to a value
class Expression {
public:
    virtual ~Expression() = default;
    virtual void accept(ExpressionVisitor& visitor) const = 0;
};

/// A constant expression that is just a literal value
/// Example: `myVar + 1` -> `1` is a LiteralExpression
class LiteralExpression : public Expression {
public:
    LiteralExpression(std::unique_ptr<GameEnvironment::Value> value)
        : value(std::move(value)) {}
    
    void accept(ExpressionVisitor& visitor) const override {
        visitor.visit(*this);
    }

    std::unique_ptr<GameEnvironment::Value> value;
};

/// An expression that identifies the value of a variable
/// Example: `myVar + 1` -> `myVar` is an IdentifierExpression
/// Example: `!myCond`   -> `myCond` is an IdentifierExpression
class IdentifierExpression : public Expression {
public:
    IdentifierExpression() = default;
    IdentifierExpression(GameEnvironment::Identifier identifier) : identifier(identifier){}

    void accept(ExpressionVisitor& visitor) const override {
        visitor.visit(*this);
    }

    GameEnvironment::Identifier identifier;
};

/// A binary expression that operates on a left and right Expression with the given operator
/// Example:  `myVar + 1` -> The entire statement is a BinaryExpression operating on two Expressions with a + operator
class BinaryExpression : public Expression {
public:
    BinaryExpression(std::unique_ptr<Expression> leftOperand, std::unique_ptr<Expression> rightOperand, Operator op)
    : leftOperand(std::move(leftOperand)), rightOperand(std::move(rightOperand)), op(op)
    {}

    void accept(ExpressionVisitor& visitor) const override {
        visitor.visit(*this);
    }

    std::unique_ptr<Expression> leftOperand;
    std::unique_ptr<Expression> rightOperand;
    Operator op;
};

/// A unary expression that operates on a single Expression with the given operator
/// Example: `!myCond` -> The entire statement is a UnaryExpression operating on a single Expression with a ! operator
class UnaryExpression : public Expression {
public:
    UnaryExpression(std::unique_ptr<Expression> operand, Operator op)
    : operand(std::move(operand)), op(op)
    {}

    void accept(ExpressionVisitor& visitor) const override {
        visitor.visit(*this);
    }

    std::unique_ptr<Expression> operand;
    Operator op;
};

// A sequence of 1 or more period-delimited identifiers (e.g. "players.elements.weapon")
class QualifiedIdentifier {
public:
    QualifiedIdentifier() = default;
    QualifiedIdentifier(std::string_view qualifiedIdentifier) {
        // Note: can't use std::isstream to easily split tokens since we we are using string_view
        size_t pos = qualifiedIdentifier.find('.');

        while (pos != std::string_view::npos) {
            identifiers.push_back(qualifiedIdentifier.substr(0, pos));
            qualifiedIdentifier.remove_prefix(pos + 1);
            pos = qualifiedIdentifier.find('.');
        }

        identifiers.push_back(qualifiedIdentifier);
    }

    void accept(ExpressionVisitor& visitor) const {
        visitor.visit(*this);
    }
    
    std::vector<GameEnvironment::Identifier> identifiers;
};

#endif