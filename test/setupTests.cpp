#include <gtest/gtest.h>
#include <variant>
#include <map>
#include <string>
#include <string_view>
#include "GameSetupLoader.h"


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

//TEST(ConvertToRangeTest, InvalidInput) {
//    EXPECT_THROW(convertToRange("(1,10"), std::invalid_argument);
//    EXPECT_THROW(convertToRange("1,10)"), std::invalid_argument);
//    EXPECT_THROW(convertToRange("(1;10)"), std::invalid_argument);
//    EXPECT_THROW(convertToRange("(1, 10, 15)"), std::invalid_argument);
//}

TEST(ConvertToChoiceListTest, ValidInput) {
    ChoiceList expected{{"apple", "fruit"}, {"banana", "fruit"}};
    ChoiceList result = convertToChoiceList("'apple','fruit','banana','fruit'");
    EXPECT_EQ(expected, result);
}

//TEST(ConvertToChoiceListTest, InvalidInput) {
//    EXPECT_THROW(convertToChoiceList("'apple','fruit,'banana','fruit'"), std::runtime_error);
//    EXPECT_THROW(convertToChoiceList("'apple','fruit','banana,'fruit"), std::runtime_error);
//    EXPECT_THROW(convertToChoiceList("apple,fruit,banana,fruit"), std::runtime_error);
//}

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

//TEST(ConvertStringToDomainTest, InvalidInput) {
//    EXPECT_THROW(convertStringToDomain(KIND::INTEGER, "(1,10,15)"), std::invalid_argument);
//    EXPECT_THROW(convertStringToDomain(KIND::ENUM, "'apple','fruit,'banana','fruit'"), std::runtime_error);
//    EXPECT_THROW(convertStringToDomain(static_cast<KIND>(42), "(1,10)"), std::runtime_error); // Example of an unsupported KIND
//}
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


