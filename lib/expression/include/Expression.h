#ifndef EXPRESSION__H
#define EXPRESSION__H

#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <variant>
#include <unordered_map>

/// @brief The valid types of a constant expression
using ValidTypes = std::variant<
  int,
  bool,
  std::string_view,
  std::vector<int>,
  std::vector<bool>,
  std::vector<std::string_view>,
  std::unordered_map<std::string_view, std::string_view>,
  std::unordered_map<std::string_view, int>,
  std::unordered_map<std::string_view, bool>
>;

/// @brief Checks if a type is contained in a container or assignable to a type in the container
template<typename T, class TContainer>
struct is_contained_assignable_in {};

/// @brief Checks if a type is contained or assignable in a container (e.x. a variant container)
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
  static constexpr bool value = (
    // evaluate using fold expression over the TContainerTs
    // for each type in TContainer, check if it is the same or assignable to T
    // then OR the result agains the evaluation of the next type in the container ("|| ...")
    // - https://www.modernescpp.com/index.php/from-variadic-templates-to-fold-expressions
    (std::is_same_v<T, TContainerTs> || std::is_assignable_v<TContainerTs, T>) || ...
  );
};

/// @brief A constant expression that is just a value
/// @tparam T The type of the value
template<typename T>
class ConstantExpression;

/// @brief Base expression factory class
class Expression {
public:
    virtual ~Expression() = default;
    /// @brief Get the value of the expression
    /// @return A variant of the valid types
    virtual ValidTypes getValue() = 0;

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
  static
  std::unique_ptr<T> dynamicPtrCast(std::unique_ptr<S>&& ptr) {
    auto converted = std::unique_ptr<T>(dynamic_cast<T*>(ptr.get()));
    if(converted) ptr.release();
    return converted;
  }
};

template<typename T>
class ConstantExpression : public Expression {
public:
    ConstantExpression(T value) : m_value(ValidTypes(value)) {
        static_assert(
            is_contained_assignable_in<T, ValidTypes>::value,
            "Invalid type for ValueExpression"
        );
    }

    ValidTypes getValue() override { return m_value; }

private:
    ValidTypes m_value;
};

#endif