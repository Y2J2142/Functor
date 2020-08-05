#pragma once
#include "Traits.hpp"
#include <algorithm>
#include <exception>
#include <fmt/core.h>
#include <iterator>
#include <numeric>
#include <type_traits>
#include <utility>
namespace Functional {

struct ChunkToBigException : std::exception {
	const char *what() const noexcept {
		return "Chunk cannot be bigger than size of the collection";
	}
};

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
	requires RegularCallable<Func, ValueType> Functor<ReturnFunctor>
		Map(Func &&f) const {
		ReturnFunctor outFunctor{};
		if constexpr (requires { this->collection.reserve(1); })
			outFunctor.reserve(outFunctor.size());

		std::transform(collection.begin(), collection.end(),
					   std::back_inserter(outFunctor), std::forward<Func>(f));

		return Functor<ReturnFunctor>{std::move(outFunctor)};
	}

	template <typename Func, typename ValueType = T::value_type,
			  typename Ret = ClearTypeT<std::invoke_result_t<Func, ValueType>>>
	requires std::is_same_v<Ret, ValueType> &&Iterable<T>
		Functor Map(Func &&f) && {
		std::transform(this->begin(), this->end(), this->begin(),
					   std::forward<Func>(f));
		return *this;
	}

	template <typename Func>
	requires UnaryPredicate<Func, typename T::value_type>
		Functor Filter(Func &&f) const {
		T filtered{};
		std::copy_if(this->collection.begin(), this->collection.end(),
					 std::back_inserter(filtered), std::forward<Func>(f));

		return Functor{std::move(filtered)};
	}

	template <typename Func>
	requires UnaryPredicate<Func, typename T::value_type> &&Erasable<T>
		Functor Filter(Func &&f) && {
		collection.erase(std::remove_if(this->begin(), this->end(),
										std::not_fn(std::forward<Func>(f))),
						 this->end());
		return *this;
	}

	template <typename Func, typename ValueType = T::value_type,
			  typename Accumulator =
				  ClearTypeT<std::invoke_result_t<Func, ValueType, ValueType>>>
	Accumulator Reduce(Func &&f, Accumulator &&acc = Accumulator{}) const {
		return std::accumulate(this->begin(), this->end(),
							   std::forward<Accumulator>(acc));
	}

	Functor<SwapTemplateParameterT<T, T>> Chunk(std::size_t chunkSize) const & {
		if (chunkSize > this->size())
			throw ChunkToBigException{};
		SwapTemplateParameterT<T, T> outCollection{};
		if constexpr (requires { outCollection.reserve(1); })
			outCollection.reserve(this->size() / chunkSize + 1);
		auto chunkBegin = this->begin();
		auto chunkEnd = chunkBegin;
		do {
			if (std::distance(chunkEnd, this->end()) < chunkSize)
				chunkEnd = this->end();
			else
				std::advance(chunkEnd, chunkSize);

			outCollection.emplace_back(chunkBegin, chunkEnd);
			chunkBegin = chunkEnd;
		} while (std::distance(chunkBegin, this->end()) > 0);

		return Functor<SwapTemplateParameterT<T, T>>{outCollection};
	}

	Functor<SwapTemplateParameterT<T, T>> Chunk(std::size_t chunkSize) && {
		if (chunkSize > this->size())
			throw ChunkToBigException{};

		SwapTemplateParameterT<T, T> outCollection{};
		if constexpr (requires { outCollection.reserve(1); })
			outCollection.reserve(this->size() / chunkSize + 1);
		auto chunkBegin = this->begin();
		auto chunkEnd = chunkBegin;
		do {
			if (std::distance(chunkEnd, this->end()) < chunkSize)
				chunkEnd = this->end();
			else
				std::advance(chunkEnd, chunkSize);
			auto chunk = std::vector<typename T::value_type>{};
			if constexpr (requires { outCollection.reserve(1); })
				chunk.reserve(chunkSize);
			std::move(chunkBegin, chunkEnd, std::back_inserter(chunk));
			outCollection.emplace_back(std::move(chunk));
			chunkBegin = chunkEnd;
		} while (std::distance(chunkBegin, this->end()) > 0);

		return Functor<SwapTemplateParameterT<T, T>>{outCollection};
	}

	bool Any() const noexcept { return this->size() != 0; }

	bool Any(const T::value_type &t) const noexcept
		requires EqualityComparable<T> {
		return std::any_of(this->begin(), this->end(),
						   [&t](auto &&i) { return i == t; });
	}

	template <typename Func>
	bool Any(Func &&f) const
		requires UnaryPredicate<Func, typename T::value_type> {
		return std::any_of(this->begin(), this->end(), std::forward<Func>(f));
	}
};

template <Collection T>
auto From(T &&t) {
	return Functor{std::forward<T>(t)};
}

} // namespace Functional