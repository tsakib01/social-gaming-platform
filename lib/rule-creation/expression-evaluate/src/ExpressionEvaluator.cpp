#include "ExpressionEvaluator.h"

#include <iostream>

class ExpressionEvaluateVisitor : public ExpressionVisitor {
public:
    ExpressionEvaluateVisitor(
        const Evaluator& evaluator,
        const GameState& gameState) :
        m_evaluator(evaluator),
        m_gameState(gameState) {}
        
    
    std::unique_ptr<GameEnvironment::Value>
    visit(const LiteralExpression& expression) override {
        return std::make_unique<GameEnvironment::Value>(expression.value->value);
    }

    std::unique_ptr<GameEnvironment::Value>
    visit(const IdentifierExpression& expression) override {

    }

    std::unique_ptr<GameEnvironment::Value>
    visit(const BinaryExpression& expression) override {

    }

    std::unique_ptr<GameEnvironment::Value>
    visit(const UnaryExpression& expression) override {

    }

    std::unique_ptr<GameEnvironment::Value>
    visit(const QualifiedIdentifier& expression) override {

    }

    std::unique_ptr<GameEnvironment::Value>
    visit([[maybe_unused]] const Expression& expression) override {
        throw std::runtime_error("Case Unreachable");
    }

private:
    const Evaluator& m_evaluator;
    const GameState& m_gameState;
};

ExpressionEvaluator::ExpressionEvaluator(const GameState& state, const Evaluator& evaluator)
    : m_gameState(state), m_evaluator(evaluator) {}

std::unique_ptr<GameEnvironment::Value>
ExpressionEvaluator::evaluate(Expression& expression) {
    auto visitor = ExpressionEvaluateVisitor(m_evaluator, m_gameState);
    return expression.accept(visitor);
}
