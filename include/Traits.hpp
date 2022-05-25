#pragma once
#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <iterator>
#include <ranges>
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
	using Type = Base<NewType>;
};
template <typename NewType, template <typename...> typename Base,
		  typename OldType>
struct SwapTemplateParameter<NewType, Base<OldType>> {
	using Type = Base<NewType>;
};

template <typename T, typename U>
using SwapTemplateParameterT = typename SwapTemplateParameter<T, U>::Type;

template <typename...>
struct IsStdArray : std::false_type {};

template <typename T, std::size_t N>
struct IsStdArray<std::array<T, N>> : std::true_type {};

template <typename... T>
constexpr inline bool IsStdArrayV = IsStdArray<T...>::value;

template <typename T>
concept Bool = std::is_convertible_v<T, bool> || std::is_same_v<T, bool>;

template <typename T>
concept TrueType = std::is_base_of_v<std::true_type, T> &&
	std::is_convertible_v<T, std::true_type>;

template <typename T>
concept FalseType = std::is_base_of_v<std::false_type, T> &&
	std::is_convertible_v<T, std::false_type>;

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

template <typename Func, typename... Args>
concept Callable = requires(Func &&f, Args &&...args) {
	std::invoke(std::forward<Func>(f), std::forward<Args>(args)...);
};

template <typename Func, typename... Args>
concept RegularCallable = Callable<Func, Args...>;

template <class From, class To>
concept convertible_to = std::is_convertible_v<From, To> &&
	requires(std::add_rvalue_reference_t<From> (&f)()) {
	static_cast<To>(f());
};

template <typename Func, typename Arg>
concept UnaryPredicate = requires(Func f, Arg arg) {
	{ f(arg) } -> convertible_to<bool>;
};

template <typename T>
concept ConstIterable = requires(T t) {
	typename T::const_iterator;
};

template <typename T>
concept Iterable = requires(T t) {
	typename T::iterator;
};

template <typename T>
concept Erasable = ConstIterable<T> && requires(T t) {
	t.erase(typename T::const_iterator{}, typename T::const_iterator{});
};

template <typename T>
concept InnerValue = requires {
	typename T::value_type;
};

template <typename T>
using ClearTypeT = std::remove_cvref_t<T>;

template <typename T>
concept Collection = requires(T t) {
	t.begin();
	t.end();
	t.size();
	requires NotTypeV<IsStdArray<T>>;
	requires InnerValue<ClearTypeT<T>>;
};

template <typename T>
concept EqualityComparable = requires(T t) {
	t == t;
};

template <typename T, typename A, typename B>
concept BinaryPredicate = requires(T t, A a, B b) {
	{ t(a, b) } -> convertible_to<bool>;
};

template <typename T>
concept StdSortable = requires(T t) {
	std::ranges::sort(t);
};

template <typename T>
concept MethodSortable = requires(T t) {
	t.sort();
};

template <typename T>
concept Sortable = MethodSortable<T> || StdSortable<T>;

template <typename T>
concept HasEmplaceBack = Collection<T> && requires(T t) {
	{ t.emplace_back() } -> std::same_as<typename T::value_type &>;
};

template <typename T>
concept Nestable = Collection<T> &&
	std::is_constructible_v<SwapTemplateParameterT<T, T>> && HasEmplaceBack<T>;

template <typename T>
concept Indexable = Collection<T> && requires(T t) {
	{ t[0] } -> std::same_as<typename T::value_type &>;
};

template <typename T>
concept Averageable = requires(T t) {
	{t + t};
	{t / size_t{}};
};

template <class...>
struct Flatten {};

template <Collection T>
struct Flatten<T> {
	using Type = Flatten<typename T::value_type>::Type;
};
template <class T>
struct Flatten<T> {
	using Type = T;
};

template <class T, class FlatContainer>
void flatten(const T &t, FlatContainer &flat) {

	if constexpr (Collection<T>) {
		for (auto elem : t) {
			flatten(elem, flat);
		}
	} else {
		flat.push_back(t);
	}
}

template <class T, class Flat = Flatten<T>::Type>
auto flatten(const T &t) {
	SwapTemplateParameterT<Flat, T> flatContainer{};
	flatten(t, flatContainer);
	return flatContainer;
}

template <class T, class... Ts>
requires(std::equality_comparable_with<T, Ts> &&...) bool allEqual(T &&t,
																   Ts &&...ts) {
	return ((std::forward<T>(t) == std::forward<Ts>(ts)) && ...);
}
} // namespace Functional
