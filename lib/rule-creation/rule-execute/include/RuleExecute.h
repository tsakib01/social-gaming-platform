#pragma once

#include "Rule.h"
#include "GameState.h"

/// Context for executing a rule
struct ExecuteContext {
    GameState& gameState;
};

/// Represents a rule executor. Requires the rule and the context to execute.
class Execute {
public:
    void execute(Rule& rule, ExecuteContext& context) {
        executeImpl(rule, context);
    }

private:
    virtual void executeImpl(Rule& rule, ExecuteContext& context) = 0;
};

#define RULE_EXECUTE void executeImpl(Rule& rule, ExecuteContext& context) override;

class BodyRuleExecute final : public Execute {
private: RULE_EXECUTE
};
class ForRuleExecute final : public Execute {
private: RULE_EXECUTE
};
class MatchRuleExecute final : public Execute {
private: RULE_EXECUTE
};
class DiscardRuleExecute final : public Execute {
private: RULE_EXECUTE
};
class MessageRuleExecute final : public Execute {
private: RULE_EXECUTE
};
class ParallelForRuleExecute final : public Execute {
private: RULE_EXECUTE
};
class InputChoiceRuleExecute final : public Execute {
private: RULE_EXECUTE
};
class ExtendRuleExecute final : public Execute {
private: RULE_EXECUTE
};

/// A wrapper over a map of {string -> Execute} where the string is the typeid of a Rule.
class RuleExecutor {
public:
    using Executors = std::map<std::string_view, std::unique_ptr<Execute>>;

    RuleExecutor(Executors& executors) : executors(std::move(executors)) {}

    /// Executes a rule using the appropriate executor.
    /// @tparam TRule The type of the rule - the key of the executor map.
    template <typename TRule>
    void executeRule(Rule& rule, ExecuteContext& context) {
        auto executor = executors[typeid(TRule).name()].get();
        executor->execute(rule, context);
    }

    /// Create a default executor map.
    static Executors createExecutorMap() {
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

private:
    Executors executors;
};
