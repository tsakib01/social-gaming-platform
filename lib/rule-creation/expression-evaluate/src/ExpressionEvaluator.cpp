#include "ExpressionEvaluator.h"

#include <iostream>


ExpressionEvaluateVisitor::ExpressionEvaluateVisitor(
    const Evaluator& evaluator,
    const GameState& gameState
) : m_evaluator(evaluator), m_gameState(gameState) {}
    

std::unique_ptr<GameEnvironment::Value>
ExpressionEvaluateVisitor::operator()(const LiteralExpression& expression) {
    (void)expression;
    return std::make_unique<GameEnvironment::Value>();
}

std::unique_ptr<GameEnvironment::Value>
ExpressionEvaluateVisitor::operator()(const IdentifierExpression& expression) {
    (void)expression;
    return std::make_unique<GameEnvironment::Value>();
}
std::unique_ptr<GameEnvironment::Value>
ExpressionEvaluateVisitor::operator()(const BinaryExpression& expression) {
    (void)expression;
    return std::make_unique<GameEnvironment::Value>();
}
std::unique_ptr<GameEnvironment::Value>
ExpressionEvaluateVisitor::operator()(const UnaryExpression& expression) {
    (void)expression;
    return std::make_unique<GameEnvironment::Value>();
}

std::unique_ptr<GameEnvironment::Value>
ExpressionEvaluateVisitor::operator()([[maybe_unused]] const Expression& expression) {
    throw std::runtime_error("Case Unreachable");
}
