#include <gtest/gtest.h>
#include "Expression.h"

template<typename T>
void assertPrimitiveValue(Value value, T expected) {
  auto primitiveVal = std::get<Primitive>(value);
  ASSERT_EQ(std::get<T>(primitiveVal), expected);
}

template<typename T>
void assertListValuePrimitive(List::value_type value, T expected) {
  auto primitiveVal = std::get<Primitive>(value);
  ASSERT_EQ(std::get<T>(primitiveVal), expected);
}

TEST(ExpressionTests, CreateConstIntExpression) {
  auto expr = Expression::createConstExpr(5);
  auto value = expr->getValue();
  assertPrimitiveValue(value, 5);
}

TEST(ExpressionTests, CreateConstBoolExpression) {
  auto trueExpr = Expression::createConstExpr(true);
  auto value = trueExpr->getValue();
  assertPrimitiveValue(value, true);

  auto falseExpr = Expression::createConstExpr(false);
  value = falseExpr->getValue();
  assertPrimitiveValue(value, false);
}

TEST(ExpressionTests, CreateConstStringExpression) {
  auto str1 = Expression::createConstExpr<std::string_view>("test1");
  auto value = str1->getValue();
  assertPrimitiveValue(value, std::string_view("test1"));

  auto str2 = std::string("test2");
  auto strExpr1 = Expression::createConstExpr<std::string_view>(str2);
  value = strExpr1->getValue();
  assertPrimitiveValue(value, std::string_view("test2"));

  auto str3 = "test3";
  auto strExpr2 = Expression::createConstExpr<std::string_view>(str3);
  value = strExpr2->getValue();
  assertPrimitiveValue(value, std::string_view("test3"));

  std::string_view strview = "test4";
  auto strExpr4 = Expression::createConstExpr(strview);
  value = strExpr4->getValue();
  assertPrimitiveValue(value, std::string_view("test4"));
}

TEST(ExpressionTests, CreateConstPrimitiveListExpression) {
  auto list = List();
  list.push_back(5);
  list.push_back(true);
  list.push_back(std::string_view("test"));
  auto expectedSize = list.size();

  auto listExpr = Expression::createConstExpr(list);
  auto value = listExpr->getValue();

  auto listVal = std::get<List>(value);
  ASSERT_EQ(listVal.size(), expectedSize);

  assertListValuePrimitive(listVal[0], 5);
  assertListValuePrimitive(listVal[1], true);
  assertListValuePrimitive(listVal[2], std::string_view("test"));
}

TEST(ExpressionTests, CreateConstListOfMapExpression) {
  auto list = List();
  auto map1 = Map();
  map1.insert(std::make_pair("m1key1", 5));
  map1.insert(std::make_pair("m1key2", true));
  auto map1ExpectedSize = map1.size();

  auto map2 = Map();
  map2.insert(std::make_pair("m2key1", std::string_view("test")));
  map2.insert(std::make_pair("m2key2", 10));
  auto map2ExpectedSize = map2.size();

  list.push_back(map1);
  list.push_back(map2);
  auto expectedSize = list.size();

  auto listExpr = Expression::createConstExpr(list);
  auto value = listExpr->getValue();

  auto listVal = std::get<List>(value);
  ASSERT_EQ(listVal.size(), expectedSize);

  auto mapVal1 = std::get<Map>(listVal[0]);
  ASSERT_EQ(mapVal1.size(), map1ExpectedSize);
  ASSERT_EQ(std::get<int>(mapVal1["m1key1"]), 5);
  ASSERT_EQ(std::get<bool>(mapVal1["m1key2"]), true);

  auto mapVal2 = std::get<Map>(listVal[1]);
  ASSERT_EQ(mapVal2.size(), map2ExpectedSize);
  ASSERT_EQ(std::get<std::string_view>(mapVal2["m2key1"]), std::string_view("test"));
  ASSERT_EQ(std::get<int>(mapVal2["m2key2"]), 10);
}

TEST(ExpressionTests, CreateConstMapExpression) {
  auto map = Map();
  map.insert(std::make_pair("key1", 5));
  map.insert(std::make_pair("key2", true));
  map.insert(std::make_pair("key3", std::string_view("test")));
  auto expectedSize = map.size();

  auto mapExpr = Expression::createConstExpr(map);
  auto value = mapExpr->getValue();

  auto mapVal = std::get<Map>(value);
  ASSERT_EQ(mapVal.size(), expectedSize);
  auto val1 = mapVal["key1"];
  ASSERT_EQ(std::get<int>(mapVal["key1"]), 5);
  ASSERT_EQ(std::get<bool>(mapVal["key2"]), true);
  ASSERT_EQ(std::get<std::string_view>(mapVal["key3"]), std::string_view("test"));
}