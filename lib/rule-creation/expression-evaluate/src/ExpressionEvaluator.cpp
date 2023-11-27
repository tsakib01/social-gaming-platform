#include "ExpressionEvaluator.h"

#include <iostream>

class ExpressionEvaluateVisitor : public ExpressionVisitor {
public:
    ExpressionEvaluateVisitor(
        const Evaluator& evaluator,
        const GameState& gameState) :
        m_evaluator(evaluator),
        m_gameState(gameState),
        m_result(std::make_unique<GameEnvironment::Value>()) {}
        
    
    void visit(const LiteralExpression& expression) const override {
        m_result->value = expression.value->value;
    }

    void visit(const IdentifierExpression& expression) const override {

    }

    void visit(const BinaryExpression& expression) const override {

    }

    void visit(const UnaryExpression& expression) const override {

    }

    void visit(const QualifiedIdentifier& expression) const override {

    }

    void visit([[maybe_unused]] const Expression& expression) const override {
        throw std::runtime_error("Case Unreachable");
    }

    std::unique_ptr<GameEnvironment::Value> getResult() {
        return std::move(m_result);
    }

private:
    const Evaluator& m_evaluator;
    const GameState& m_gameState;
    std::unique_ptr<GameEnvironment::Value> m_result;
};

ExpressionEvaluator::ExpressionEvaluator(const GameState& state, const Evaluator& evaluator)
    : m_gameState(state), m_evaluator(evaluator) {}

std::unique_ptr<GameEnvironment::Value>
ExpressionEvaluator::evaluate(const Expression& expression) const {
    auto visitor = ExpressionEvaluateVisitor(m_evaluator, m_gameState);
    expression.accept(visitor);
    return visitor.getResult();
}
