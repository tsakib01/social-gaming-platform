#include "RuleExecute.h"
#include <iostream>

void BodyRuleExecute::executeImpl(Rule& rule, ExecuteContext& context) {
    (void)rule;
    (void)context;
}

void ForRuleExecute::executeImpl(Rule& rule, ExecuteContext& context) {
    (void)rule;
    (void)context;
}

void MatchRuleExecute::executeImpl(Rule& rule, ExecuteContext& context) {
    (void)rule;
    (void)context;
}

void DiscardRuleExecute::executeImpl(Rule& rule, ExecuteContext& context) {
    (void)rule;
    (void)context;
}

void MessageRuleExecute::executeImpl(Rule& rule, ExecuteContext& context) {
    (void)rule;
    (void)context;
}

void ParallelForRuleExecute::executeImpl(Rule& rule, ExecuteContext& context) {
    (void)rule;
    (void)context;
}

void InputChoiceRuleExecute::executeImpl(Rule& rule, ExecuteContext& context) {
    (void)rule;
    (void)context;
}

void ExtendRuleExecute::executeImpl(Rule& rule, ExecuteContext& context) {
    (void)rule;
    (void)context;
}

void RuleExecutor::executeRule(Rule& rule, ExecuteContext& context) {
    auto executor = m_executors[typeid(rule).name()].get();
    executor->execute(rule, context);
}

RuleExecutor::Executors RuleExecutor::createDefault() {
    auto executorMap = Executors{};
    executorMap.emplace(typeid(BodyRule).name(), std::make_unique<BodyRuleExecute>());
    executorMap.emplace(typeid(ForRule).name(), std::make_unique<ForRuleExecute>());
    executorMap.emplace(typeid(MatchRule).name(), std::make_unique<MatchRuleExecute>());
    executorMap.emplace(typeid(DiscardRule).name(), std::make_unique<DiscardRuleExecute>());
    executorMap.emplace(typeid(MessageRule).name(), std::make_unique<MessageRuleExecute>());
    executorMap.emplace(typeid(ParallelForRule).name(), std::make_unique<ParallelForRuleExecute>());
    executorMap.emplace(typeid(InputChoiceRule).name(), std::make_unique<InputChoiceRuleExecute>());
    executorMap.emplace(typeid(ExtendRule).name(), std::make_unique<ExtendRuleExecute>());
    return executorMap;
}
