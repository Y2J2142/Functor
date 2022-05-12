#pragma once
#include "Traits.hpp"
#include <cstdint>
#include <exception>
#include <type_traits>
#include <utility>
namespace Functional {

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

	template <typename Func>
	requires std::is_invocable_v<Func, ValueType>
	auto Apply(Func &&func) {
		if (this->hasValue)
			return Monad{func(this->value)};
		return Monad{};
	}
};

template <typename T>
Monad(T t) -> Monad<std::remove_cvref_t<T>>;

template <typename T>
Monad(Monad<T> t) -> Monad<std::remove_cvref_t<T>>;
} // namespace Functional
