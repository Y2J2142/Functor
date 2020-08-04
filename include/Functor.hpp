#pragma once
#include "Traits.hpp"
#include <algorithm>
#include <fmt/core.h>
#include <iterator>
#include <type_traits>
#include <utility>
namespace Functional {

template <Collection T>
class Functor {

	T collection;

  public:
	Functor() = default;
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
			  typename ReturnFunctor = SwapTemplateParameterT<Ret, T>>
	Functor<ReturnFunctor> Map(Func &&f) const {
		ReturnFunctor outFunctor{};
		if constexpr (requires { this->collection.reserve(1); })
			outFunctor.reserve(outFunctor.size());

		std::transform(collection.begin(), collection.end(),
					   std::back_inserter(outFunctor), std::forward<Func>(f));

		return Functor<ReturnFunctor>{std::move(outFunctor)};
	}
};

template <Collection T>
auto From(T &&t) {
	return Functor{std::forward<T>(t)};
}

} // namespace Functional