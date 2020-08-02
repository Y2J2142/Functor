#pragma once
#include "Traits.hpp"
#include <algorithm>
#include <type_traits>
#include <utility>
namespace Functional {

template <typename T>
concept Collection = requires(T t) {
	t.begin();
	t.end();
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

	Functor(T &&t) : collection{std::move(t)} {}
	Functor(const T &t) : collection{t} {}
	template <typename Func, typename ValueType = T::value_type,
			  typename Ret =
				  std::remove_cvref_t<std::invoke_result_t<Func, ValueType>>,
			  typename ReturnFunctor = SwapTemplateParameter<Ret, T>::Type>
	ReturnFunctor Map(Func &&f) {
		Ret outFunctor{};

		return ReturnFunctor{};
	}
};

template <Collection T>
auto From(T &&t) {
	return Functor{std::forward<T>(t)};
}

} // namespace Functional