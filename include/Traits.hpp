#pragma once
#include <array>
#include <cstddef>
#include <type_traits>
namespace Functional {

template <typename Test, template <typename...> typename Ref>
struct IsSpecialization : std::false_type {};

template <template <typename...> class Ref, typename... Args>
struct IsSpecialization<Ref<Args...>, Ref> : std::true_type {};

template <typename...>
struct SwapTemplateParameter {};

template <typename NewType, template <typename...> typename Base,
		  typename OldType, typename... Args>
struct SwapTemplateParameter<NewType, Base<OldType, Args...>> {
	using Type = Base<NewType, Args...>;
};
template <typename NewType, template <typename...> typename Base,
		  typename OldType>
struct SwapTemplateParameter<NewType, Base<OldType>> {
	using Type = Base<NewType>;
};

template <typename...>
struct IsStdArray : std::false_type {};

template <typename T, std::size_t N>
struct IsStdArray<std::array<T, N>> : std::true_type {};

template <typename... T>
constexpr inline bool IsStdArrayV = IsStdArray<T...>::value;

template <typename T>
concept Bool = std::is_convertible_v<T, bool> || std::is_same_v<T, bool>;

template <typename T>
concept TrueType = std::is_base_of_v<std::true_type, T>
	&&std::is_convertible_v<T, std::true_type>;

template <typename T>
concept FalseType = std::is_base_of_v<std::false_type, T>
	&&std::is_convertible_v<T, std::false_type>;

template <Bool T>
constexpr inline bool Not(T &&t) {
	return !std::forward<T>(t);
}

template <typename T>
concept BooleanType = TrueType<T> || FalseType<T>;

template <typename T, typename = void>
struct NotType {};

template <TrueType T>
struct NotType<T> : std::false_type {};

template <FalseType T>
struct NotType<T> : std::true_type {};

template <BooleanType T>
constexpr inline bool NotTypeV = NotType<T>::value;

} // namespace Functional
