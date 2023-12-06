#include <gtest/gtest.h>
#include <variant>
#include <map>
#include <string>
#include <string_view>
#include "GameSetupLoader.h"
#include "GameConfigLoader.h"
#include "GameSetup.h"
#include <fstream>

class DomainCheckerTest : public ::testing::Test {
protected:
    DomainChecker checker;
};
TEST_F(DomainCheckerTest, ReturnsTrueForVariantWithInRange) {
    Domain domain = Range{1, 10};
    auto checkerFunc = [this](auto&& arg) { return checker(arg, std::string_view("5")); };
    EXPECT_TRUE(std::visit(checkerFunc, domain));
}

TEST_F(DomainCheckerTest, ReturnsFalseForVariantWithOutOfRange) {
    Domain domain = Range{1, 10};
    auto checkerFunc = [this](auto&& arg) { return checker(arg, std::string_view("20")); };
    EXPECT_FALSE(std::visit(checkerFunc, domain));
}

TEST_F(DomainCheckerTest, ReturnsTrueForVariantWithChoiceInList) {
    Domain domain = ChoiceList{{"apple", "fruit"}, {"carrot", "vegetable"}};
    auto checkerFunc = [this](auto&& arg) { return checker(arg, std::string_view("carrot")); };
    EXPECT_TRUE(std::visit(checkerFunc, domain));
}

TEST_F(DomainCheckerTest, ReturnsFalseForVariantWithChoiceNotInList) {
    Domain domain = ChoiceList{{"apple", "fruit"}, {"carrot", "vegetable"}};
    auto checkerFunc = [this](auto&& arg) { return checker(arg, std::string_view("potato")); };
    EXPECT_FALSE(std::visit(checkerFunc, domain));
}


TEST(ConvertToKINDTest, ValidKinds) {
    EXPECT_EQ(convertToKIND("integer"), KIND::INTEGER);
    EXPECT_EQ(convertToKIND("string"), KIND::STRING);
    EXPECT_EQ(convertToKIND("boolean"), KIND::BOOLEAN);
    EXPECT_EQ(convertToKIND("enum"), KIND::ENUM);
}

TEST(ConvertToKINDTest, InvalidKinds) {
    EXPECT_THROW(convertToKIND("unsupported"), std::runtime_error);
    EXPECT_THROW(convertToKIND(""), std::runtime_error);
    EXPECT_THROW(convertToKIND("int"), std::runtime_error);
}

TEST(KINDToStringTest, ValidKinds) {
    EXPECT_EQ(KINDToString(KIND::INTEGER), "integer");
    EXPECT_EQ(KINDToString(KIND::STRING), "string");
    EXPECT_EQ(KINDToString(KIND::BOOLEAN), "boolean");
    EXPECT_EQ(KINDToString(KIND::ENUM), "enum");
}

TEST(KINDToStringTest, InvalidKinds) {
    EXPECT_THROW(KINDToString(static_cast<KIND>(-1)), std::runtime_error);
}

TEST(ConvertToRangeTest, ValidInput) {
    Range expected{1, 10};
    Range result = convertToRange("(1,10)");
    EXPECT_EQ(expected, result);
}

TEST(ConvertToRangeTest, InvalidInput) {
    EXPECT_THROW(convertToRange("(1,10"), std::invalid_argument);
    EXPECT_THROW(convertToRange("1,10)"), std::invalid_argument);
    EXPECT_THROW(convertToRange("(1;10)"), std::invalid_argument);
    EXPECT_THROW(convertToRange("(1, 10, 15)"), std::invalid_argument);
}

TEST(ConvertToChoiceListTest, ValidInput) {
    ChoiceList expected{{"apple", "fruit"}, {"banana", "fruit"}};
    ChoiceList result = convertToChoiceList("'apple','fruit','banana','fruit'");
    EXPECT_EQ(expected, result);
}

TEST(ConvertToChoiceListTest, InvalidInput) {
    EXPECT_THROW(convertToChoiceList("'apple','fruit,'banana','fruit'"), std::runtime_error);
    EXPECT_THROW(convertToChoiceList("'apple','fruit','banana,'fruit"), std::runtime_error);
    EXPECT_THROW(convertToChoiceList("apple,fruit,banana,fruit"), std::runtime_error);
}

TEST(ConvertStringToDomainTest, ValidInput) {
    KIND kind = KIND::INTEGER;
    std::string_view restInfo = "(1, 10)";
    Domain rangeResult = convertStringToDomain(kind, restInfo);
    EXPECT_TRUE(std::holds_alternative<Range>(rangeResult));
    EXPECT_EQ((std::get<Range>(rangeResult)), (Range{1, 10}));

    Domain choicesResult = convertStringToDomain(KIND::ENUM, "'apple','fruit','banana','fruit'");
    ASSERT_TRUE(std::holds_alternative<ChoiceList>(choicesResult)); // Make sure the variant holds a ChoiceList
    ChoiceList expected = {{"apple", "fruit"}, {"banana", "fruit"}};
    ChoiceList actual = std::get<ChoiceList>(choicesResult);
    EXPECT_EQ(expected, actual);
}

TEST(ConvertStringToDomainTest, InvalidInput) {
    EXPECT_THROW(convertStringToDomain(KIND::INTEGER, "(1,10,15)"), std::invalid_argument);
    EXPECT_THROW(convertStringToDomain(KIND::ENUM, "'apple','fruit,'banana','fruit'"), std::runtime_error);
    EXPECT_THROW(convertStringToDomain(static_cast<KIND>(42), "(1,10)"), std::runtime_error); // Example of an unsupported KIND
}
TEST(SetupInstanceTest, CombinedTestForAllKinds) {
    // Testing KIND::INTEGER
    SetupInstance setupInt("int_test", KIND::INTEGER, "Enter a number between 1 and 10", "", Domain(Range{1, 10}));
    EXPECT_TRUE(setupInt.checkResponse("5"));
    EXPECT_FALSE(setupInt.checkResponse("20"));

    // Testing KIND::STRING
    SetupInstance setupStr("str_test", KIND::STRING, "Enter any text");
    EXPECT_TRUE(setupStr.checkResponse("any text"));

    // Testing KIND::BOOLEAN
    SetupInstance setupBool("bool_test", KIND::BOOLEAN, "Enter 'y' or 'n'");
    EXPECT_TRUE(setupBool.checkResponse("y"));
    EXPECT_TRUE(setupBool.checkResponse("n"));
    EXPECT_FALSE(setupBool.checkResponse("yes"));
    EXPECT_FALSE(setupBool.checkResponse("no"));

    // Testing KIND::ENUM
    SetupInstance setupEnum("enum_test", KIND::ENUM, "Choose a fruit", "", Domain(ChoiceList{{"apple", ""}, {"banana", ""}}));
    EXPECT_TRUE(setupEnum.checkResponse("apple"));
    EXPECT_FALSE(setupEnum.checkResponse("cherry"));
}

//SetupLoaderTest
extern "C" {
    TSLanguage* tree_sitter_socialgaming();
}

class configParserTest{
public:
    configParserTest(std::string path){
        std::ifstream ifs(path);
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        ifs.close();
        source = buffer.str();
    }
    std::string source;
};

TEST(SingleSetupParserTest, intSetup ){
    configParserTest config ("./test/games/setup-parser-test.game");
    auto setupLoader= GameSetupLoader(config.source);
    ts::Language language = tree_sitter_socialgaming();
    ts::Parser parser{language};
    ts::Tree tree = parser.parseString(config.source);
    ts::Node root = tree.getRootNode();
    ts::Node configuration = root.getChildByFieldName("configuration");
    ts::Node setupNode = configuration.getNamedChild(3);
    std::unique_ptr<SetupInstance> setupPtr = std::move(setupLoader.convertNodetoSetup(setupNode));
    EXPECT_EQ(setupPtr->getIdentifier(), "rounds");
    EXPECT_EQ(setupPtr->getKind(), KIND::INTEGER);
    EXPECT_EQ(setupPtr->getPrompt(), "\"The number of rounds to play\"");
    EXPECT_EQ(setupPtr->getRestInfo(), "range: (10, 20)");
}

TEST(SingleSetupParserTest, boolSetup ){
    configParserTest config ("./test/games/setup-parser-test.game");
    auto setupLoader= GameSetupLoader(config.source);
    ts::Language language = tree_sitter_socialgaming();
    ts::Parser parser{language};
    ts::Tree tree = parser.parseString(config.source);
    ts::Node root = tree.getRootNode();
    ts::Node configuration = root.getChildByFieldName("configuration");
    ts::Node setupNode = configuration.getNamedChild(4);
    std::unique_ptr<SetupInstance> setupPtr = setupLoader.convertNodetoSetup(setupNode);
    EXPECT_EQ(setupPtr->getIdentifier(), "public_voting");
    EXPECT_EQ(setupPtr->getKind(), KIND::BOOLEAN);
    EXPECT_EQ(setupPtr->getPrompt(), "\"Player votes are public\"");
    EXPECT_EQ(setupPtr->getRestInfo(), "");
}

TEST(SingleSetupParserTest, stringSetup ){
    configParserTest config ("./test/games/setup-parser-test.game");
    auto setupLoader= GameSetupLoader(config.source);
    ts::Language language = tree_sitter_socialgaming();
    ts::Parser parser{language};
    ts::Tree tree = parser.parseString(config.source);
    ts::Node root = tree.getRootNode();
    ts::Node configuration = root.getChildByFieldName("configuration");
    ts::Node setupNode = configuration.getNamedChild(5);
    std::unique_ptr<SetupInstance> setupPtr = setupLoader.convertNodetoSetup(setupNode);
    EXPECT_EQ(setupPtr->getIdentifier(), "string_example");
    EXPECT_EQ(setupPtr->getKind(), KIND::STRING);
    EXPECT_EQ(setupPtr->getPrompt(), "\"Do nothing\"");
    EXPECT_EQ(setupPtr->getRestInfo(), "");
}

TEST(SingleSetupParserTest, enumSetup ){
    configParserTest config ("./test/games/setup-parser-test.game");
    auto setupLoader= GameSetupLoader(config.source);
    ts::Language language = tree_sitter_socialgaming();
    ts::Parser parser{language};
    ts::Tree tree = parser.parseString(config.source);
    ts::Node root = tree.getRootNode();
    ts::Node configuration = root.getChildByFieldName("configuration");
    ts::Node setupNode = configuration.getNamedChild(6);
    std::unique_ptr<SetupInstance> setupPtr = setupLoader.convertNodetoSetup(setupNode);
    EXPECT_EQ(setupPtr->getIdentifier(), "mode");
    EXPECT_EQ(setupPtr->getKind(), KIND::ENUM);
    EXPECT_EQ(setupPtr->getPrompt(), "\"Game Style\"");
    EXPECT_EQ(setupPtr->getRestInfo(), "choices: {\r\n        'fast':     'A quick round with friends'\r\n        'standard': 'Standard play'\r\n        'long':     'A marathon battle against former friends'\r\n      }");
}

TEST(SingleSetupParserTest, invalidSetup_wrongType ){
    configParserTest config ("./test/games/setup-parser-invalid-test.game");
    auto setupLoader= GameSetupLoader(config.source);
    ts::Language language = tree_sitter_socialgaming();
    ts::Parser parser{language};
    ts::Tree tree = parser.parseString(config.source);
    ts::Node root = tree.getRootNode();
    ts::Node configuration = root.getChildByFieldName("configuration");
    ts::Node setupNode = configuration.getNamedChild(0);
    EXPECT_THROW(setupLoader.convertNodetoSetup(setupNode), std::runtime_error);
}

TEST(SingleSetupParserTest, invalidSetup_lessInfo ){
    configParserTest config ("./test/games/setup-parser-invalid-test.game");
    auto setupLoader= GameSetupLoader(config.source);
    ts::Language language = tree_sitter_socialgaming();
    ts::Parser parser{language};
    ts::Tree tree = parser.parseString(config.source);
    ts::Node root = tree.getRootNode();
    ts::Node configuration = root.getChildByFieldName("configuration");
    ts::Node setupNode = configuration.getNamedChild(1);
    EXPECT_THROW(setupLoader.convertNodetoSetup(setupNode), std::runtime_error);
}

TEST(AllSetupParserTest, validGameFile ){
    configParserTest config ("./test/games/setup-parser-test.game");
    auto gameSetupLoader = std::make_shared<GameSetupLoader>(config.source);
    auto gameSetup = GameSetup(gameSetupLoader);
    ts::Language language = tree_sitter_socialgaming();
    ts::Parser parser{language};
    ts::Tree tree = parser.parseString(config.source);
    ts::Node root = tree.getRootNode();
    ts::Node configuration = root.getChildByFieldName("configuration");
    gameSetup.addSetups(configuration);
    std::vector<std::string_view> identifiers = {"rounds","public_voting","string_example","mode"};
    std::vector<std::string_view> prompts = {"\"The number of rounds to play\"","\"Player votes are public\"","\"Do nothing\"","\"Game Style\""};
    std::vector<std::string_view> restInfos = {"range: (10, 20)","","","choices: {\r\n        'fast':     'A quick round with friends'\r\n        'standard': 'Standard play'\r\n        'long':     'A marathon battle against former friends'\r\n      }"};
    EXPECT_EQ(gameSetup.getIdentifiers(), identifiers);
    EXPECT_EQ(gameSetup.getPrompts(), prompts);
    EXPECT_EQ(gameSetup.getRestInfos(), restInfos);
}

TEST(AllSetupParserTest, invalidGameFile ){
    configParserTest config ("./test/games/setup-parser-invalid-test.game");
    auto gameSetupLoader = GameSetupLoader(config.source);
    ts::Language language = tree_sitter_socialgaming();
    ts::Parser parser{language};
    ts::Tree tree = parser.parseString(config.source);
    ts::Node root = tree.getRootNode();
    ts::Node configuration = root.getChildByFieldName("configuration");
    EXPECT_THROW(gameSetupLoader.getGameSetup(configuration), std::runtime_error);
}
