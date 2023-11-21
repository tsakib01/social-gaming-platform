#include "RuleExecute.h"
#include <iostream>

void RuleExecuteVisitor::visit(BodyRule& rule) {
    std::cout << "executing body rule" << std::endl;
    auto index = context.gameState.getValue(&rule);
    if (index < rule.rules.size()) {
        context.instructionStack.push(rule.rules.at(index).get());
        context.gameState.updateState(&rule, index + 1);
        return;
    }
    context.gameState.updateState(&rule, static_cast<size_t>(-1));
    context.instructionStack.pop();
}

void RuleExecuteVisitor::visit(ForRule& rule) {
    (void)rule;
    // Pop while unimplemented
    std::cout << "executing for rule" << std::endl;
    context.instructionStack.pop();
}

void RuleExecuteVisitor::visit(MatchRule& rule) {
    (void)rule;
}

void RuleExecuteVisitor::visit(DiscardRule& rule) {
    (void)rule;
}

void RuleExecuteVisitor::visit(MessageRule& rule) {
    (void)rule;
}

void RuleExecuteVisitor::visit(ParallelForRule& rule) {
    (void)rule;
}

void RuleExecuteVisitor::visit(InputChoiceRule& rule) {
    (void)rule;
}

void RuleExecuteVisitor::visit(ExtendRule& rule) {
    (void)rule;
}

void RuleExecuteVisitor::visit(ScoresRule& rule) {
    (void)rule;
    std::cout << "executing scores rule" << std::endl;
    // Pop while unimplemented
    context.instructionStack.pop();
}

void RuleExecuteVisitor::visit([[maybe_unused]] Rule& rule) {
    throw std::runtime_error("Case Unreachable");
}
