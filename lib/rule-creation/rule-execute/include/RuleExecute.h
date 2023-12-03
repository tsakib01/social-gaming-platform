#pragma once

#include "Rule.h"
#include "GameState.h"
#include <stack>
#include "Evaluator.h"
#include "ExpressionEvaluator.h"

/// Context for executing a rule
struct ExecuteContext {
    GameState& gameState;
    std::stack<Rule*> instructionStack;
    bool blocked = false;

    ExecuteContext(GameState& state, Rule* initialRule)
        : gameState(state) {
        instructionStack.push(initialRule);
    }
};

class RuleExecuteVisitor : public RuleVisitor {
public:
    RuleExecuteVisitor(ExecuteContext& context)
        : context(context), eval(), exprVisitor(eval, context.gameState) {
        eval.defaultEvaluatorFactory();
    }

    void visit(BodyRule& rule) override;
    void visit(ForRule& rule) override;
    void visit(MatchRule& rule) override;
    void visit(DiscardRule& rule) override;
    void visit(MessageRule& rule) override;
    void visit(ParallelForRule& rule) override;
    void visit(InputChoiceRule& rule) override;
    void visit(ExtendRule& rule) override;
    void visit(ScoresRule& rule) override;
    void visit(Rule& rule) override;

private:
    ExecuteContext& context;
    Evaluator eval;
    ExpressionEvaluateVisitor exprVisitor;
};
