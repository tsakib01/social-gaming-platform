#include "ExpressionEvaluator.h"

#include <iostream>


ExpressionEvaluateVisitor::ExpressionEvaluateVisitor(
    const Evaluator& evaluator,
    const GameState& gameState
) : m_evaluator(evaluator), m_gameState(gameState) {}
    

std::unique_ptr<GameEnvironment::Value>
ExpressionEvaluateVisitor::operator()(const LiteralExpression& expression) {
    return std::make_unique<GameEnvironment::Value>(*expression.value);
}

std::unique_ptr<GameEnvironment::Value>
ExpressionEvaluateVisitor::operator()(const IdentifierExpression& expression) {
    return std::make_unique<GameEnvironment::Value>(expression.identifier);
}

std::unique_ptr<GameEnvironment::Value>
ExpressionEvaluateVisitor::operator()(const BinaryExpression& expression) {
    std::unique_ptr<GameEnvironment::Value> leftOperand = expression.leftOperand->accept(*this);
    std::unique_ptr<GameEnvironment::Value> rightOperand = expression.rightOperand->accept(*this);
    // Handles dot operation, other operations not handled yet
    if (expression.op == Operator::DOT) {
        if (auto builtinExpression = dynamic_cast<BuiltinExpression*>(expression.rightOperand.get())) {
            // Right Operand is a BuiltinExpression, handle it here
            return handleBuiltinExpression(*leftOperand, *builtinExpression);
        } else {
            // Left Operand should be a map and right Operand is a key
            auto leftOperandIdentifier = std::get_if<std::string_view>(&leftOperand->value);
            if (!leftOperandIdentifier) {
                throw std::runtime_error("Dot expression with non-identifer left value");
            }
            auto leftValue = m_gameState.getValue(*leftOperandIdentifier);
            auto rightOperandIdentifier = std::get_if<std::string_view>(&rightOperand->value);
            auto* leftMapPtr = std::get_if<std::unique_ptr<GameEnvironment::Map>>(&leftValue.value);
            if (!rightOperandIdentifier || !leftMapPtr || !*leftMapPtr) {
                throw std::runtime_error("Error resolving dot expression");
            }
            auto value = leftMapPtr->get()->at(*rightOperandIdentifier).get();
            return std::make_unique<GameEnvironment::Value>(*value);
        }
    }
    return std::make_unique<GameEnvironment::Value>();
}

std::unique_ptr<GameEnvironment::Value>
ExpressionEvaluateVisitor::operator()(const UnaryExpression& expression) {
    (void)expression;
    return std::make_unique<GameEnvironment::Value>();
}

std::unique_ptr<GameEnvironment::Value>
ExpressionEvaluateVisitor::operator()(const BuiltinExpression& expression) {
    (void)expression;
    return std::make_unique<GameEnvironment::Value>();
}

std::unique_ptr<GameEnvironment::Value>
ExpressionEvaluateVisitor::operator()([[maybe_unused]] const Expression& expression) {
    throw std::runtime_error("Case Unreachable");
}


std::unique_ptr<GameEnvironment::Value>
ExpressionEvaluateVisitor::handleBuiltinExpression(const GameEnvironment::Value& value, const BuiltinExpression& expression) {
    switch (expression.builtin) {
        case Builtin::UPFROM: {
            std::unique_ptr<GameEnvironment::Value> argument = expression.arguments.front()->accept(*this);
            return std::make_unique<GameEnvironment::Value>(m_evaluator.evaluate(OPERATOR::UPFROM, {&value, argument.get()}));
        }
        case Builtin::SIZE: {
            // Handle the SIZE case
            // Implement logic for SIZE

            break;
        }
        case Builtin::CONTAINS: {
            // Handle the CONTAINS case
            // Implement logic for CONTAINS

            break;
        }
        case Builtin::COLLECT: {
            // Handle the COLLECT case
            // Implement logic for COLLECT

            break;
        }
        // Add more cases as needed...

        default: {
            break;
        }
    }
    return std::make_unique<GameEnvironment::Value>();
}