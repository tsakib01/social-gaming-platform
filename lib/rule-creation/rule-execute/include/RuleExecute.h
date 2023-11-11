#pragma once

#include "Rule.h"
#include "GameState.h"

/// Context for executing a rule
struct ExecuteContext {
    GameState& gameState;
};

/// Represents a rule executor. Requires the rule and the context to execute.
/// @tparam TRule The type of the rule to execute.
template <typename TRule>
class Execute {
public:
    virtual void execute(TRule& rule, ExecuteContext& context) = 0;
};

#define RULE_EXECUTE(TRule) void execute(TRule& rule, ExecuteContext& context) override;

class BodyRuleExecute final : public Execute<BodyRule> {
public: RULE_EXECUTE(BodyRule)
};
class ForRuleExecute final : public Execute<ForRule> {
public: RULE_EXECUTE(ForRule)
};
class MatchRuleExecute final : public Execute<MatchRule> {
public: RULE_EXECUTE(MatchRule)
};
class DiscardRuleExecute final : public Execute<DiscardRule> {
public: RULE_EXECUTE(DiscardRule)
};
class MessageRuleExecute final : public Execute<MessageRule> {
public: RULE_EXECUTE(MessageRule)
};
class ParallelForRuleExecute final : public Execute<ParallelForRule> {
public: RULE_EXECUTE(ParallelForRule)
};
class InputChoiceRuleExecute final : public Execute<InputChoiceRule> {
public: RULE_EXECUTE(InputChoiceRule)
};
class ExtendRuleExecute final : public Execute<ExtendRule> {
public: RULE_EXECUTE(ExtendRule)
};

class RuleExecuteVisitor : public RuleVisitor {
public:
    RuleExecuteVisitor(ExecuteContext& context)
        : context(context) {}

    void visit(BodyRule& rule) override { BodyRuleExecute{}.execute(rule, context); }
    void visit(ForRule& rule) override { ForRuleExecute{}.execute(rule, context); }
    void visit(MatchRule& rule) override { MatchRuleExecute{}.execute(rule, context); }
    void visit(DiscardRule& rule) override { DiscardRuleExecute{}.execute(rule, context); }
    void visit(MessageRule& rule) override { MessageRuleExecute{}.execute(rule, context); }
    void visit(ParallelForRule& rule) override { ParallelForRuleExecute{}.execute(rule, context); }
    void visit(InputChoiceRule& rule) override { InputChoiceRuleExecute{}.execute(rule, context); }
    void visit(ExtendRule& rule) override { ExtendRuleExecute{}.execute(rule, context); }

    void visit([[maybe_unused]] Rule& rule) override {
        throw std::runtime_error("Case Unreachable");
    }

private:
    ExecuteContext& context;
};
