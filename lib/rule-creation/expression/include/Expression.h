#ifndef EXPRESSION__H
#define EXPRESSION__H

#include <string_view>
#include <vector>
#include <memory>
#include <variant>
#include <map>
#include "GameEnvironment.h"

/// Checks if a type is the same or assignable to another type
/// @tparam T The type to check
/// @tparam S The type to check against
template<typename T, typename S>
struct is_same_or_assignable {
    static constexpr bool value = std::is_same_v<T, S> || std::is_assignable_v<S, T>;
};

/// Checks if a type is contained in a container or assignable to a type in the container.
/// Container refers to std::variant, std::vector, std::map, or any other type containing elements that
/// implements a specialization of this template.
template<typename T, class TContainer>
struct is_contained_assignable_in : std::false_type {};

/// Checks if a type is contained or assignable in a container (e.x. an std::variant container)
/// @tparam T The type to check
/// @tparam TContainer The container to check against
template<
  typename T,
  template<typename...> class TContainer,
  // packed parameter expansion of TContainer's types (e.x. TContainer<T1, T2, T3> -> T1, T2, T3)
  // - https://en.cppreference.com/w/cpp/language/parameter_pack
  typename... TContainerTs
>
struct is_contained_assignable_in<T, TContainer<TContainerTs...>> {
    static constexpr bool value = ((
        // evaluate using fold expression over the TContainerTs
        // for each type in TContainer, check if it is the same or assignable to T
        // then OR the result agains the evaluation of the next type in the container ("|| ...")
        // - https://www.modernescpp.com/index.php/from-variadic-templates-to-fold-expressions
        is_same_or_assignable<T, TContainerTs>::value
    ) || ...);
};

/// Checks if a type is a map type with valid string key and Primitive value types
/// @tparam TMap The type to check
/// @tparam TKey The type of the map's keys
/// @tparam TValue The type of the map's values
template<typename TKey, typename TValue>
struct is_contained_assignable_in<std::map<TKey, TValue>, GameEnvironment::Map> {
    static constexpr bool value = (
        // check if the key type is the same or assignable to std::string_view
        is_same_or_assignable<TKey, GameEnvironment::Value>::value
    );
};

/// Checks if a type is a vector type with valid element types
/// @tparam TElm The type of the vector's elements
template<typename TElm>
struct is_contained_assignable_in<std::vector<TElm>, GameEnvironment::List> {
    static constexpr bool value = (
        // check if the element type is contained or assignable in Primitive
        is_contained_assignable_in<TElm, GameEnvironment::Value>::value
    );
};

/// Operations that can be applied to expression(s).
enum class Operator{
    PLUS, SUBTRACT, MULTIPLY, DIVISION, OR, IDENTIFIER, DOT
};

/// An expression interface that represents a statement in the game config that can be evaluated to a value
class Expression {};

/// A constant expression that is just a literal value
/// @tparam T The type of the value
/// Example: `myVar + 1` -> `1` is a LiteralExpression
template<typename T>
class LiteralExpression : public Expression {
public:
    LiteralExpression(T value) {
        // static_assert(
        //     is_contained_assignable_in<T, GameEnvironment::Value>::value,
        //     "Invalid type for LiteralExpression"
        // );
        this->value.value = value;
        std::cout << std::get<bool>(this->value.value) << '\n';
    }
    GameEnvironment::Value value;
};

/// An expression that identifies the value of a variable
/// Example: `myVar + 1` -> `myVar` is an IdentifierExpression
/// Example: `!myCond`   -> `myCond` is an IdentifierExpression
class IdentifierExpression : public Expression {
public:
    IdentifierExpression() = default;
    IdentifierExpression(GameEnvironment::Identifier identifier) : identifier(identifier){}
    GameEnvironment::Identifier identifier;
};

/// A binary expression that operates on a left and right Expression with the given operator
/// Example:  `myVar + 1` -> The entire statement is a BinaryExpression operating on two Expressions with a + operator
class BinaryExpression : public Expression {
public:
    BinaryExpression(std::unique_ptr<Expression> leftOperand, std::unique_ptr<Expression> rightOperand, Operator op)
    : leftOperand(std::move(leftOperand)), rightOperand(std::move(rightOperand)), op(op)
    {}

    std::unique_ptr<Expression> leftOperand;
    std::unique_ptr<Expression> rightOperand;
    Operator op;
};

/// A unary expression that operates on a single Expression with the given operator
/// Example: `!myCond` -> The entire statement is a UnaryExpression operating on a single Expression with a ! operator
class UnaryExpression : public Expression {
public:
    UnaryExpression(std::unique_ptr<Expression> operand, Operator op)
    : operand(std::move(operand)), op(op)
    {}

    std::unique_ptr<Expression> operand;
    Operator op;
};

#endif