#include "RuleExecute.h"
#include <iostream>

void RuleExecuteVisitor::visit(BodyRule& rule) {
    (void)rule;
}

void RuleExecuteVisitor::visit(ForRule& rule) {
    (void)rule;
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

void visit([[maybe_unused]] Rule& rule) {
    throw std::runtime_error("Case Unreachable");
}
