#include "Functor.hpp"
#include "Identity.hpp"
#include "catch2/catch.hpp"
#include <fmt/core.h>
#include <list>
#include <type_traits>
#include <vector>
using namespace Functional;

int triple(int i) { return i * 3; }

TEST_CASE("Functor construction", "[functor]") {
	std::vector ints{1, 2, 3, 4, 5};
	auto fromInts = From(ints);
}

TEST_CASE("Functor map", "[functor]") {
	auto functor = From(std::vector{1, 2, 3, 4, 5});
	auto func = [](int i) { return i * 2; };
	auto mapped = functor.Map(func).Map(triple);
	REQUIRE(mapped.UnderlyingCollection() == std::vector{6, 12, 18, 24, 30});
	auto arr = Functor{std::vector<int>{1, 2, 3, 4, 5}};
	auto toFloats = arr.Map([](int i) { return static_cast<float>(i); });
	REQUIRE(std::is_same_v<decltype(toFloats), Functor<std::vector<float>>>);
}

TEST_CASE("Filter", "[functor]") {
	auto functor = From(std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
	auto evens = functor.Filter([](auto i) { return i % 2 == 0; });
	REQUIRE(functor.UnderlyingCollection() ==
			std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
	REQUIRE(evens.UnderlyingCollection() == std::vector{2, 4, 6, 8, 10});
	auto even = [](int i) { return i % 2 == 0; };
	auto inPlace =
		Functor{std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}}.Filter(even);
	REQUIRE(inPlace.UnderlyingCollection() == std::vector{2, 4, 6, 8, 10});
}

TEST_CASE("Reduce", "[functor]") {
	auto sum = 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10;
	auto acc = From(std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10})
				   .Reduce([](int a, int b) { return a + b; });
	REQUIRE(sum == acc);
}

TEST_CASE("Chunk", "[functor]") {

	auto functor = Functor{std::list{1, 2, 3, 4, 5, 6, 7, 8}};
	auto chunked = functor.Chunk(4);
	REQUIRE(chunked.size() == 2);
	REQUIRE(chunked.UnderlyingCollection().front() == std::list{1, 2, 3, 4});
	REQUIRE(chunked.UnderlyingCollection().back() == std::list{5, 6, 7, 8});
	REQUIRE(
		std::is_same_v<decltype(chunked), Functor<std::list<std::list<int>>>>);
	auto rvalueChunked =
		From(std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}).Chunk(1);
	REQUIRE(rvalueChunked.size() == 10);
}
