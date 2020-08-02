#include "Functor.hpp"
#include "catch2/catch.hpp"
#include <fmt/core.h>
#include <type_traits>
#include <vector>
using namespace Functional;

TEST_CASE("Functor construction", "[functor]") {
	std::vector ints{1, 2, 3, 4, 5};
	auto functor = From(ints);
	auto func = [](int i) { return i * 2; };
	auto mapped = functor.Map(func);
}