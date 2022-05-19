#pragma once
#include "Traits.hpp"
#include <algorithm>
#include <exception>
#include <fmt/core.h>
#include <iterator>
#include <list>
#include <numeric>
#include <stdexcept>
#include <type_traits>
#include <utility>
namespace Functional {

struct EmptyFunctorException : std::exception {
	const char *what() const noexcept { return "Functor is empty"; }
};

template <Collection T>
class Functor {

	T collection;

  public:
	using ValueType = typename T::value_type;
	Functor() = default;
	decltype(auto) begin() const noexcept { return collection.begin(); }
	decltype(auto) begin() noexcept { return collection.begin(); }
	decltype(auto) end() const noexcept { return collection.end(); }
	decltype(auto) end() noexcept { return collection.end(); }
	auto size() const noexcept { return collection.size(); }
	auto Size() const noexcept { return collection.size(); }
	const T &UnderlyingCollection() const noexcept { return collection; }
	T &UnderlyingCollection() noexcept { return collection; }
	Functor(T &&t) : collection{std::move(t)} {}
	Functor(const T &t) : collection{t} {}

	ValueType &operator[](size_t idx) noexcept requires Indexable<T> {
		return collection[idx];
	}

	const ValueType &
		operator[](size_t idx) const noexcept requires Indexable<T> {
		return collection[idx];
	}

	template <typename Func,
			  typename Ret =
				  std::remove_cvref_t<std::invoke_result_t<Func, ValueType>>,
			  typename ReturnFunctor = SwapTemplateParameterT<Ret, T>>
	requires RegularCallable<Func, ValueType> Functor<ReturnFunctor>
		Map(Func &&f)
	const {
		ReturnFunctor outFunctor{};
		if constexpr (requires { this->collection.reserve(1); })
			outFunctor.reserve(outFunctor.size());

		std::transform(collection.begin(), collection.end(),
					   std::back_inserter(outFunctor),
					   [&f]<typename Arg>(Arg &&arg) {
						   return std::invoke(std::forward<Func>(f),
											  std::forward<Arg>(arg));
					   });

		return Functor<ReturnFunctor>{std::move(outFunctor)};
	}

	template <typename Func, typename ValueType = T::value_type,
			  typename Ret = ClearTypeT<std::invoke_result_t<Func, ValueType>>>
	requires std::is_same_v<Ret, ValueType> && Iterable<T>
		Functor Map(Func &&f) && {
		std::transform(this->begin(), this->end(), this->begin(),
					   [&f]<typename Arg>(Arg &&arg) {
						   return std::invoke(std::forward<Func>(f),
											  std::forward<Arg>(arg));
					   });
		return *this;
	}

	template <typename Func>
	requires UnaryPredicate<Func, typename T::value_type>
		Functor Filter(Func &&f)
	const {
		T filtered{};
		std::copy_if(this->collection.begin(), this->collection.end(),
					 std::back_inserter(filtered), std::forward<Func>(f));

		return Functor{std::move(filtered)};
	}

	template <typename Func>
	requires UnaryPredicate<Func, typename T::value_type> && Erasable<T>
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
	auto Chunk(std::size_t chunkSize) const &requires Nestable<T> {
		SwapTemplateParameterT<T, T> outCollection{};
		if constexpr (requires { outCollection.reserve(1); })
			outCollection.reserve(this->size() / chunkSize + 1);
		auto chunkBegin = this->begin();
		auto chunkEnd = chunkBegin;
		do {
			if (static_cast<std::size_t>(std::distance(chunkEnd, this->end())) <
				chunkSize)
				chunkEnd = this->end();
			else
				std::advance(chunkEnd, chunkSize);

			outCollection.emplace_back(chunkBegin, chunkEnd);
			chunkBegin = chunkEnd;
		} while (std::distance(chunkBegin, this->end()) > 0);

		return Functor<SwapTemplateParameterT<T, T>>{outCollection};
	}
	auto Chunk(std::size_t chunkSize) && requires Nestable<T> {
		SwapTemplateParameterT<T, T> outCollection{};
		if constexpr (requires { outCollection.reserve(1); })
			outCollection.reserve(this->size() / chunkSize + 1);
		auto chunkBegin = this->begin();
		auto chunkEnd = chunkBegin;
		do {
			if (static_cast<std::size_t>(std::distance(chunkEnd, this->end())) <
				chunkSize)
				chunkEnd = this->end();
			else
				std::advance(chunkEnd, chunkSize);
			auto chunk = T{};
			if constexpr (requires { outCollection.reserve(1); })
				chunk.reserve(chunkSize);
			std::move(chunkBegin, chunkEnd, std::back_inserter(chunk));
			outCollection.emplace_back(std::move(chunk));
			chunkBegin = chunkEnd;
		} while (std::distance(chunkBegin, this->end()) > 0);

		return Functor<SwapTemplateParameterT<T, T>>{outCollection};
	}

	template <template <typename...> typename C>
	requires std::is_constructible_v<C<T>> && Collection<C<T>>
	auto Chunk(std::size_t chunkSize) const & {

		C<T> outCollection{};
		if constexpr (requires { outCollection.reserve(1); })
			outCollection.reserve(this->size() / chunkSize + 1);
		auto chunkBegin = this->begin();
		auto chunkEnd = chunkBegin;
		do {
			if (static_cast<std::size_t>(std::distance(chunkEnd, this->end())) <
				chunkSize)
				chunkEnd = this->end();
			else
				std::advance(chunkEnd, chunkSize);

			outCollection.emplace_back(chunkBegin, chunkEnd);
			chunkBegin = chunkEnd;
		} while (std::distance(chunkBegin, this->end()) > 0);

		return Functor<C<T>>{outCollection};
	}

	template <template <typename...> typename C>
	requires std::is_constructible_v<C<T>> && Collection<C<T>>
	auto Chunk(std::size_t chunkSize) && {
		C<T> outCollection{};
		if constexpr (requires { outCollection.reserve(1); })
			outCollection.reserve(this->size() / chunkSize + 1);
		auto chunkBegin = this->begin();
		auto chunkEnd = chunkBegin;
		do {
			if (static_cast<std::size_t>(std::distance(chunkEnd, this->end())) <
				chunkSize)
				chunkEnd = this->end();
			else
				std::advance(chunkEnd, chunkSize);
			auto chunk = T{};
			if constexpr (requires { outCollection.reserve(1); })
				chunk.reserve(chunkSize);
			std::move(chunkBegin, chunkEnd, std::back_inserter(chunk));
			outCollection.emplace_back(std::move(chunk));
			chunkBegin = chunkEnd;
		} while (std::distance(chunkBegin, this->end()) > 0);

		return Functor<C<T>>{outCollection};
	}

	bool Any() const noexcept { return this->size() != 0; }

	bool Any(const ValueType &t) const noexcept requires EqualityComparable<T> {
		return std::any_of(this->begin(), this->end(),
						   [&t](auto &&i) { return i == t; });
	}

	template <typename Func>
	bool Any(Func &&f) const requires
		UnaryPredicate<Func, typename T::value_type> {
		return std::any_of(this->begin(), this->end(), std::forward<Func>(f));
	}

	std::size_t Count() const noexcept { return this->size(); }
	std::size_t Count(const typename T::value_type &t) const noexcept {
		return std::count(this->begin(), this->end(), t);
	}
	template <typename Func>
	requires UnaryPredicate<Func, typename T::value_type> std::size_t
		Count(Func &&f)
	const {
		return std::count_if(this->begin(), this->end(), std::forward<Func>(f));
	}
	template <typename Func = std::less<>>
	requires BinaryPredicate<Func, typename T::value_type,
							 typename T::value_type> && StdSortable<T>
		Functor Sort(Func &&f = std::less<>{})
	const & {
		T sorted{};
		if constexpr (requires(T t) { t.reserve(1); })
			sorted.reserve(this->size());
		std::copy(this->begin(), this->end(), std::back_inserter(sorted));
		std::sort(sorted.begin(), sorted.end(), std::forward<Func>(f));
		return Functor{std::move(sorted)};
	}

	template <typename Func = std::less<>>
	requires BinaryPredicate<Func, typename T::value_type,
							 typename T::value_type> && StdSortable<T>
		Functor Sort(Func &&f = Func{}) && {
		std::sort(this->begin(), this->end(), std::forward<Func>(f));
		return *this;
	}

	template <typename Func = std::less<>>
	requires BinaryPredicate<Func, typename T::value_type,
							 typename T::value_type> && MethodSortable<T>
		Functor Sort(Func &&f = std::less<>{})
	const & {
		T sorted(this->begin(), this->end());
		sorted.sort(std::forward<Func>(f));
		return Functor{std::move(sorted)};
	}

	template <typename Func = std::less<>>
	requires BinaryPredicate<Func, typename T::value_type,
							 typename T::value_type> && MethodSortable<T>
		Functor Sort(Func &&f = Func{}) && {
		collection.sort(std::forward<Func>(f));
		return std::move(*this);
	}

	Functor Unique() const &requires
		EqualityComparable<typename T::value_type> && Erasable<T> &&
		requires(Functor f) {
		f.Sort();
	}
	{
		auto out = this->Sort();
		auto &col = out.collection;
		col.erase(std::unique(col.begin(), col.end()), col.end());
		return out;
	}

	Functor Unique() && requires
		EqualityComparable<typename T::value_type> && Erasable<T> &&
		requires(Functor f) {
		f.Sort();
	}
	{
		this->Sort();
		this->collection.erase(std::unique(this->begin(), this->end()),
							   this->end());
		return *this;
	}

	decltype(auto) Find(const ValueType &t) {
		auto it = std::find(this->begin(), this->end(), t);
		if (it == this->end())
			throw std::runtime_error("Element not found");
		return *it;
	}

	template <typename Func>
	requires Callable<Func, ValueType> &&
		std::is_convertible_v < std::invoke_result_t<Func, ValueType>,
	bool > decltype(auto) Find(Func &&f) {
		const auto it =
			std::find_if(this->begin(), this->end(), std::forward<Func>(f));
		if (it == this->end())
			throw std::runtime_error("Element not found");
		return *it;
	}

	auto Average(const ValueType &initialValue = ValueType{}) const &requires
		Averageable<ValueType> {
		const auto acc =
			std::accumulate(this->begin(), this->end(), initialValue);
		return acc / this->Size();
	}

	template <typename Func,
			  typename AverageType = std::invoke_result_t<Func, ValueType>>
	requires std::is_invocable_v<Func, ValueType> &&
		Averageable<std::invoke_result_t<Func, ValueType>>
	auto Average(Func &&f, const AverageType &initialValue = AverageType{}) {
		auto acc = initialValue;
		for (const auto &elem : collection) {
			acc += f(elem);
		}
		return acc / this->Size();
	}

	decltype(auto) Flatten() {
		if constexpr (Collection<ValueType>) {
			auto flatCollection = flatten(collection);
			return Functor<decltype(flatCollection)>{std::move(flatCollection)};
		} else {
			return *this;
		}
	}

	template <typename Func>
	auto Collect(Func &&f) {
		return this->Map(f).Flatten();
	}
	template <class Func>
	requires UnaryPredicate<Func, ValueType>
	bool ForAll(Func &&f) {
		return std::all_of(this->begin(), this->end(), std::forward<Func>(f));
	}

	const ValueType &Head() const & {
		if (this->size() == 0)
			throw EmptyFunctorException{};
		return *begin();
	}
	ValueType &Head() & {
		if (this->size() == 0)
			throw EmptyFunctorException{};
		return *begin();
	}
	ValueType Head() && {
		if (this->size() == 0)
			throw EmptyFunctorException{};
		return std::move(*begin());
	}

	auto Tail() const & {
		if (this->size() == 0)
			throw EmptyFunctorException{};
		Functor tail{};
		std::copy(std::next(this->begin(), 1), this->end(),
				  std::back_inserter(tail.collection));
		return tail;
	}

	auto Tail() && {
		if (this->size() == 0)
			throw EmptyFunctorException{};
		collection.erase(collection.begin());
		return *this;
	}

	Functor &Add(const ValueType &value) {
		collection.push_back(value);
		return *this;
	}

	Functor Add(const ValueType &value) const {
		auto copy = T{};
		if constexpr (requires { copy.reserve(1); })
			copy.reserve(collection.size());
		std::copy(collection.begin(), collection.end(),
				  std::back_inserter(copy));
		copy.push_back(value);
		return Functor{copy};
	}

}; // namespace Functional

template <Collection T>
Functor<ClearTypeT<T>> From(T &&t) {
	return Functor{std::forward<T>(t)};
}
template <Collection C, typename T = Functor<C>>
bool operator==(const T &t, const C &o) {
	return t.UnderlyingCollection() == o;
}
} // namespace Functional