#include <gtest/gtest.h>
#include "Expression.h"

TEST(ExpressionTests, CreateConstIntExpression) {
  auto expr = Expression::createConstExpr(5);
  ASSERT_EQ(std::get<int>(expr->getValue()), 5);
}

TEST(ExpressionTests, CreateConstBoolExpression) {
  auto trueExpr = Expression::createConstExpr(true);
  ASSERT_EQ(std::get<bool>(trueExpr->getValue()), true);

  auto falseExpr = Expression::createConstExpr(false);
  ASSERT_EQ(std::get<bool>(falseExpr->getValue()), false);
}

TEST(ExpressionTests, CreateConstStringExpression) {
  auto str1 = Expression::createConstExpr<std::string_view>("test1");
  ASSERT_EQ(std::get<std::string_view>(str1->getValue()), "test1");

  auto str2 = std::string("test2");
  auto strExpr1 = Expression::createConstExpr<std::string_view>(str2);
  ASSERT_EQ(std::get<std::string_view>(strExpr1->getValue()), "test2");

  auto str3 = "test3";
  auto strExpr2 = Expression::createConstExpr<std::string_view>(str3);
  ASSERT_EQ(std::get<std::string_view>(strExpr2->getValue()), "test3");

  std::string_view strview = "test4";
  auto strExpr4 = Expression::createConstExpr(strview);
  ASSERT_EQ(std::get<std::string_view>(strExpr4->getValue()), "test4");
}

TEST(ExpressionTests, CreateConstVectorIntExpression) {
  auto vecExpr = Expression::createConstExpr<std::vector<int>>({1, 2, 3});
  ASSERT_EQ(
    std::get<std::vector<int>>(vecExpr->getValue()),
    std::vector({1, 2, 3})
  );
}

TEST(ExpressionTests, CreateConstVectorBoolExpression) {
  auto vecExpr = Expression::createConstExpr<std::vector<bool>>({true, false, true});
  ASSERT_EQ(
    std::get<std::vector<bool>>(vecExpr->getValue()),
    std::vector({true, false, true})
  );
}

TEST(ExpressionTests, CreateConstStringVectorExpression) {
  std::vector<std::string_view> expected = {"test1", "test2", "test3"};
  auto vecExpr = Expression::createConstExpr(expected);
  auto actual = std::get<std::vector<std::string_view>>(vecExpr->getValue());

  ASSERT_EQ(actual.size(), expected.size());
  for(size_t i = 0; i < actual.size(); i++) {
    ASSERT_EQ(actual[i], expected[i]);
  }
}

using StringMap = std::unordered_map<std::string_view, std::string_view>;
TEST(ExpressionTests, CreateConstStringMapExpression) {
  StringMap expected = {{"key1", "value1"}, {"key2", "value2"}};
  auto mapExpr = Expression::createConstExpr(expected);
  auto actual = std::get<StringMap>(mapExpr->getValue());

  ASSERT_EQ(actual.size(), expected.size());
  for(auto& [key, value] : actual) {
    ASSERT_EQ(value, expected[key]);
  }
}