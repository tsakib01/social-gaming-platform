#pragma once

#include "Evaluator.h"
#include "Expression.h"
#include "GameState.h"

class ExpressionEvaluator {
public:
    ExpressionEvaluator(const GameState& state, const Evaluator& evaluator);
    
    std::unique_ptr<GameEnvironment::Value>
    evaluate(const Expression& expression) const;

private:
    const GameState& m_gameState;
    const Evaluator& m_evaluator;
};
