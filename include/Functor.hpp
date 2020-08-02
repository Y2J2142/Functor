#pragma once
#include "Traits.hpp"
#include <algorithm>
#include <fmt/core.h>
#include <iterator>
#include <type_traits>
#include <utility>
namespace Functional {

template <typename T>
concept Collection = requires(T t) {
	t.begin();
	t.end();
	t.size();
	// std::remove_cvref_t<T>::value_type;
};

template <Collection T>
class Functor {

	T collection;

  public:
	decltype(auto) begin() const noexcept { return collection.begin(); }
	decltype(auto) begin() noexcept { return collection.begin(); }
	decltype(auto) end() const noexcept { return collection.end(); }
	decltype(auto) end() noexcept { return collection.end(); }
	auto size() const noexcept { return collection.size(); }
	const T &UnderlyingCollection() const noexcept { return collection; }
	T &UnderlyingCollection() noexcept { return collection; }
	Functor(T &&t) : collection{std::move(t)} {}
	Functor(const T &t) : collection{t} {}
	template <typename Func, typename ValueType = T::value_type,
			  typename Ret =
				  std::remove_cvref_t<std::invoke_result_t<Func, ValueType>>,
			  typename ReturnFunctor = SwapTemplateParameter<Ret, T>::Type>
	Functor<ReturnFunctor> Map(Func &&f) {
		ReturnFunctor outFunctor{};
		if constexpr (requires { outFunctor.reserve(1); })
			outFunctor.reserve(collection.size());

		std::transform(collection.begin(), collection.end(),
					   std::back_inserter(outFunctor), std::forward<Func>(f));

		return Functor{std::move(outFunctor)};
	}
};

template <Collection T>
auto From(T &&t) {
	return Functor{std::forward<T>(t)};
}

} // namespace Functional