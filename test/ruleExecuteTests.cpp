#include <gtest/gtest.h>
#include "RuleExecute.h"

class TestAddRule : public Rule {};

const std::string_view TestStateIdentifier = "testIdentifier";

class TestAddExecute : public Execute {
private:
    void executeImpl(Rule& rule, ExecuteContext& context) override {
        auto oldState = context.gameState.getValue(TestStateIdentifier);
        auto oldInt = std::get<int>(oldState->value);

        auto newInt = std::make_unique<GameEnvironment::Value>();
        newInt->value = oldInt + 1;
        context.gameState.updateState(TestStateIdentifier, std::move(newInt));
    }
};

TEST(RuleExecuteTests, TestExecuteRuleUpdatesGameState) {
    auto executorMap = RuleExecutor::Executors{};
    executorMap.emplace(typeid(TestAddRule).name(), std::make_unique<TestAddExecute>());
    auto executors = RuleExecutor{executorMap};
    auto gameState = GameState{};

    auto initVal = std::make_unique<GameEnvironment::Value>();
    initVal->value = 0;
    gameState.addState(TestStateIdentifier, std::move(initVal));

    auto context = ExecuteContext{gameState};
    auto testAddRule = TestAddRule{};

    executors.executeRule(testAddRule, context);
    
    auto newState = context.gameState.getValue(TestStateIdentifier);
    ASSERT_EQ(std::get<int>(newState->value), 1);

    executors.executeRule(testAddRule, context);
    
    auto newState2 = context.gameState.getValue(TestStateIdentifier);
    EXPECT_EQ(std::get<int>(newState2->value), 2);
}

TEST(RuleExecuteTests, TestExecuteCorrectRuleWithBaseType) {
    auto executorMap = RuleExecutor::Executors{};
    executorMap.emplace(typeid(TestAddRule).name(), std::make_unique<TestAddExecute>());
    auto executors = RuleExecutor{executorMap};
    auto gameState = GameState{};

    auto initVal = std::make_unique<GameEnvironment::Value>();
    initVal->value = 0;
    gameState.addState(TestStateIdentifier, std::move(initVal));

    auto context = ExecuteContext{gameState};
    auto rules = std::vector<std::unique_ptr<Rule>>();
    rules.push_back(std::make_unique<TestAddRule>());
    rules.push_back(std::make_unique<TestAddRule>());

    for (auto& rule : rules) {
        executors.executeRule(*rule, context);
    }
    
    auto newState = context.gameState.getValue(TestStateIdentifier);
    ASSERT_EQ(std::get<int>(newState->value), 2);
}
