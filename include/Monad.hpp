#pragma once
#include "Traits.hpp"
#include <cstdint>
#include <exception>
#include <type_traits>
#include <utility>
namespace Functor {

struct EmptyMonadException : public std::exception {
	const char *what() const noexcept override { return "Monad is empty"; }
};

template <typename T>
class Monad {
	std::uint8_t hasValue = 0;
	T value;

  public:
	using ValueType = T;
	Monad() = default;
	Monad(const Monad &) = default;
	Monad(Monad &&) = default;
	Monad(T t) : hasValue{1}, value{std::move(t)} {}

	Monad &operator=(const T &t) {
		this->value = t;
		this->hasValue = 1;
		return *this;
	}

	const T &Value() const {
		if (!this->hasValue)
			throw EmptyMonadException{};
		return value;
	}
	T Value() {
		if (!this->hasValue)
			throw EmptyMonadException{};
		return value;
	}

	const T &ValueOr(const T &t) const noexcept {
		if (this->hasValue)
			return this->value;
		return t;
	}

	T &ValueOr(const T &t) noexcept {
		if (this->hasValue)
			return this->value;
		return t;
	}

	bool HasValue() const noexcept { return static_cast<bool>(this->hasValue); }
};

template <typename T>
concept Monadable = is_specialization<std::remove_cvref_t<T>, Monad>::value;

template <Monadable T, typename Func,
		  typename ValueType = std::remove_cvref_t<T>::ValueType,
		  typename R = std::remove_cvref_t<std::result_of_t<Func(ValueType)>>>
requires std::is_invocable_v<Func, ValueType> Monad<R> ApplyFunc(T &&mt,
																 Func &&func) {
	Monad<R> ret{};
	if (std::forward<T>(mt).HasValue())
		ret = func(std::forward<T>(mt).Value());
	return ret;
}

template <typename T>
Monad(T t) -> Monad<std::remove_cvref_t<T>>;

template <typename T>
Monad(Monad<T> t) -> Monad<std::remove_cvref_t<T>>;
} // namespace Functor
