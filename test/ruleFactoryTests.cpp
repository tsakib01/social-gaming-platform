#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "GameConfigLoader.h"

extern "C" {
    TSLanguage* tree_sitter_socialgaming();
}

const std::string DEFAULT_CONFIG = R"(
    configuration {
    name: ""
    player range: (0, 0)
    audience: false
    setup: {}
    }

    constants {}

    variables {}

    per-player {}

    per-audience {}

)";


class MockTranslator : public Translator {
public:
    MockTranslator(std::string_view source) : Translator(source) {}

    MOCK_METHOD(std::unique_ptr<RuleTree>, translate, (const ts::Node& root), (const override));
    MOCK_METHOD(std::unique_ptr<Expression>, createExpression, (const ts::Node& node), (const override));

    void initialize() {
        registerRuleFactory("rule", std::make_unique<DummyRuleFactory>(this));
        registerRuleFactory("body", std::make_unique<BodyFactory>(this));
        registerRuleFactory("for", std::make_unique<ForFactory>(this));
        registerRuleFactory("parallel_for", std::make_unique<ParallelForFactory>(this));
        registerRuleFactory("match", std::make_unique<MatchFactory>(this));
        registerRuleFactory("discard", std::make_unique<DiscardFactory>(this));
        registerRuleFactory("message", std::make_unique<MessageFactory>(this));
        registerRuleFactory("input_choice", std::make_unique<InputChoiceFactory>(this));
        registerRuleFactory("scores", std::make_unique<ScoresFactory>(this));

        registerExpressionFactory("expression", std::make_unique<DummyExpressionFactory>(this));
        registerExpressionFactory("identifier", std::make_unique<IdentifierFactory>(this));
    }

    static ts::Tree getTree(std::string_view configData) {
        ts::Language language = tree_sitter_socialgaming();
        ts::Parser parser{language};
        return parser.parseString(configData);
    }
};

TEST(TranslatorTest, BodyRuleTest) {
    std::string configString = DEFAULT_CONFIG + R"(
    rules {
        discard winners.size() from winners;

        message all "Round {round}. Choose your weapon!";

        for player in players {
            input choice to player {
                prompt: "{player.name}, choose your weapon!"
                choices: weapons.name
                target: player.name
                timeout: 10
            }
        }
    }
})";
    ts::Tree tree = MockTranslator::getTree(configString);
    ts::Node rules = tree.getRootNode().getChildByFieldName("rules");
    MockTranslator mockTranslator(configString);
    mockTranslator.initialize();
    std::unique_ptr<Rule> rule = mockTranslator.createRule(rules.getNamedChild(0));
    BodyRule* bodyRule = (dynamic_cast<BodyRule*>(rule.release()));
    ASSERT_NE(bodyRule, nullptr);
    ASSERT_EQ(bodyRule->rules.size(), 3);
    // Check elements in BodyRule's rules
    DiscardRule* discardRule = (dynamic_cast<DiscardRule*>(bodyRule->rules.at(0).release()));
    ASSERT_NE(discardRule, nullptr);
    MessageRule* messageRule = (dynamic_cast<MessageRule*>(bodyRule->rules.at(1).release()));
    ASSERT_NE(messageRule, nullptr);
    ForRule* forRule = (dynamic_cast<ForRule*>(bodyRule->rules.at(2).release()));
    ASSERT_NE(forRule, nullptr);
}

TEST(TranslatorTest, ForRuleTest) {
    std::string configString = DEFAULT_CONFIG + R"(
    rules {
        for player in players {
            input choice to player {
                prompt: "{player.name}, choose your weapon!"
                choices: weapons.name
                target: player.name
                timeout: 10
            }
            message all "Round {round}. Choose your weapon!";
            match true {
                x = 10 => {
                    message all "is 10";
                }
                true => {
                    message all "not 10";
                }
            }
        }
    }
})";
    ts::Tree tree = MockTranslator::getTree(configString);
    ts::Node rules = tree.getRootNode().getChildByFieldName("rules");
    MockTranslator mockTranslator(configString);
    mockTranslator.initialize();
    std::unique_ptr<Rule> rule = mockTranslator.createRule(rules.getNamedChild(0));
    // Creates a body rule first as body is the first rule in a configuration
    BodyRule* bodyRule = (dynamic_cast<BodyRule*>(rule.release()));
    ForRule* forRule = (dynamic_cast<ForRule*>(bodyRule->rules.at(0).release()));
    ASSERT_NE(forRule, nullptr);
    ASSERT_EQ(forRule->currentItem.identifier, "player");
    ASSERT_EQ(forRule->body.rules.size(), 3);
    // Check elements in ForRule's BodyRule
    InputChoiceRule* inputRule = (dynamic_cast<InputChoiceRule*>(forRule->body.rules.at(0).release()));
    ASSERT_NE(inputRule, nullptr);
    MessageRule* messageRule = (dynamic_cast<MessageRule*>(forRule->body.rules.at(1).release()));
    ASSERT_NE(messageRule, nullptr);
    MatchRule* matchRule = (dynamic_cast<MatchRule*>(forRule->body.rules.at(2).release()));
    ASSERT_NE(matchRule, nullptr);
}

TEST(TranslatorTest, ParallelForRuleTest) {
    std::string configString = DEFAULT_CONFIG + R"(
    rules {
        parallel for weapon in weapons {
            input choice to player {
                prompt: "{player.name}, choose your weapon!"
                choices: weapons.name
                target: player.name
                timeout: 10
            }
            message all "Round {round}. Choose your weapon!";
            discard winners.size() from winners;
            message all "Parallel for executing!";
        }
    }
})";
    ts::Tree tree = MockTranslator::getTree(configString);
    ts::Node rules = tree.getRootNode().getChildByFieldName("rules");
    MockTranslator mockTranslator(configString);
    mockTranslator.initialize();
    std::unique_ptr<Rule> rule = mockTranslator.createRule(rules.getNamedChild(0));
    // Creates a body rule first as body is the first rule in a configuration
    BodyRule* bodyRule = (dynamic_cast<BodyRule*>(rule.release()));
    ParallelForRule* parallelForRule = (dynamic_cast<ParallelForRule*>(bodyRule->rules.at(0).release()));
    ASSERT_NE(parallelForRule, nullptr);
    ASSERT_EQ(parallelForRule->currentItem.identifier, "weapon");
    // Check elements in ParallelForRule's BodyRule
    ASSERT_EQ(parallelForRule->body.rules.size(), 4);
    InputChoiceRule* inputRule = (dynamic_cast<InputChoiceRule*>(parallelForRule->body.rules.at(0).release()));
    ASSERT_NE(inputRule, nullptr);
    MessageRule* messageRule = (dynamic_cast<MessageRule*>(parallelForRule->body.rules.at(1).release()));
    ASSERT_NE(messageRule, nullptr);
    DiscardRule* discardRule = (dynamic_cast<DiscardRule*>(parallelForRule->body.rules.at(2).release()));
    ASSERT_NE(discardRule, nullptr);
    MessageRule* messageRuleTwo = (dynamic_cast<MessageRule*>(parallelForRule->body.rules.at(3).release()));
    ASSERT_NE(messageRuleTwo, nullptr);
}