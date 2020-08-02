#include "Functor.hpp"
#include "catch2/catch.hpp"
#include <fmt/core.h>
#include <type_traits>
#include <vector>
using namespace Functional;

int triple(int i) { return i * 3; }

TEST_CASE("Functor construction", "[functor]") {
	std::vector ints{1, 2, 3, 4, 5};
	auto fromInts = From(ints);
	auto fromArray = Functor{std::array<float, 4>{1, 2, 3, 4}};
}

TEST_CASE("Functor map", "[map]") {
	auto functor = From(std::vector{1, 2, 3, 4, 5});
	auto func = [](int i) { return i * 2; };
	auto mapped = functor.Map(func).Map(triple);
	REQUIRE(mapped.UnderlyingCollection() == std::vector{6, 12, 18, 24, 30});
	auto arr = Functor{std::vector<int>{1, 2, 3, 4, 5}};
	auto toFloats = arr.Map([](int i) { return static_cast<float>(i); });
	REQUIRE(std::is_same_v<decltype(toFloats), Functor<std::vector<float>>>);
	auto fromArray = Functor{std::array<float, 4>{1, 2, 3, 4}};
	// fromArray.Map([](auto i) {return i * 3;});
}