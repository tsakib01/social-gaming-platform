#ifndef EXPRESSION__H
#define EXPRESSION__H

#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <variant>
#include <unordered_map>
#include <map>

using Primitive = std::variant<int, bool, std::string_view>;
using Map = std::map<std::string_view, Primitive>;
using List = std::vector<std::variant<Primitive, Map>>;
using Value = std::variant<Primitive, Map, List>;

template<typename T, typename S>
/// @brief Checks if a type is the same or assignable to another type
/// @tparam T The type to check
/// @tparam S The type to check against
struct is_same_or_assignable {
    static constexpr bool value = std::is_same_v<T, S> || std::is_assignable_v<S, T>;
};

template<typename T, class TContainer>
/// @brief
/// Checks if a type is contained in a container or assignable to a type in the container.
/// Container refers to std::variant, std::vector, std::map, or any other type containing elements that
/// implements a specialization of this template.
struct is_contained_assignable_in : std::false_type {};

template<
  typename T,
  template<typename...> class TContainer,
  // packed parameter expansion of TContainer's types (e.x. TContainer<T1, T2, T3> -> T1, T2, T3)
  // - https://en.cppreference.com/w/cpp/language/parameter_pack
  typename... TContainerTs
>
/// @brief Checks if a type is contained or assignable in a container (e.x. an std::variant container)
/// @tparam T The type to check
/// @tparam TContainer The container to check against
struct is_contained_assignable_in<T, TContainer<TContainerTs...>> {
    static constexpr bool value = ((
        // evaluate using fold expression over the TContainerTs
        // for each type in TContainer, check if it is the same or assignable to T
        // then OR the result agains the evaluation of the next type in the container ("|| ...")
        // - https://www.modernescpp.com/index.php/from-variadic-templates-to-fold-expressions
        is_same_or_assignable<T, TContainerTs>::value
    ) || ...);
};

template<typename TKey, typename TValue>
/// @brief Checks if a type is a map type with valid string key and Primitive value types
/// @tparam TMap The type to check
/// @tparam TKey The type of the map's keys
/// @tparam TValue The type of the map's values
struct is_contained_assignable_in<std::map<TKey, TValue>, Map> {
    static constexpr bool value = (
        // check if the key type is the same or assignable to std::string_view
        is_same_or_assignable<TKey, std::string_view>::value
    ) && (
        // check if the value type is contained or assignable in Primitive
        is_contained_assignable_in<TValue, Primitive>::value
    );
};

template<typename TElm>
/// @brief Checks if a type is a vector type with valid element types
/// @tparam TElm The type of the vector's elements
struct is_contained_assignable_in<std::vector<TElm>, List> {
    static constexpr bool value = (
        // check if the element type is contained or assignable in Primitive
        is_contained_assignable_in<TElm, Primitive>::value ||
        // check if the element type is contained or assignable in Map
        is_contained_assignable_in<TElm, Map>::value
    );
};

template<typename T>
/// @brief A constant expression that is just a value
/// @tparam T The type of the value
class ConstantExpression;

/// @brief Base expression factory class
class Expression {
public:
    virtual ~Expression() = default;
    /// @brief Get the value of the expression
    /// @return A variant of the valid types
    virtual Value getValue() = 0;

    /// @brief Create a constant expression from a value
    /// @param value The value to create the expression from
    /// @return A unique pointer to the expression
    template<typename T>
    static std::unique_ptr<Expression> 
    createConstExpr(T value) {
      auto constExpr = std::make_unique<ConstantExpression<T>>(value);
      return dynamicPtrCast<Expression>(std::move(constExpr));
    }

private:
  template<typename T, typename S>
  static std::unique_ptr<T>
  dynamicPtrCast(std::unique_ptr<S>&& ptr) {
    auto converted = std::unique_ptr<T>(dynamic_cast<T*>(ptr.get()));
    if(converted) ptr.release();
    return converted;
  }
};

template<typename T>
class ConstantExpression : public Expression {
public:
    ConstantExpression(T value) : m_value(Value(value)) {
        static_assert(
            is_contained_assignable_in<T, Value>::value,
            "Invalid type for ConstantExpression"
        );
    }

    Value getValue() override { return m_value; }

private:
    Value m_value;
};

#endif