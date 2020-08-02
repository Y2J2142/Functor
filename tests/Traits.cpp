#include "Traits.hpp"
#include "catch2/catch.hpp"
#include <fmt/core.h>
#include <vector>
using namespace Functional;
TEST_CASE("IsSpecialization testing", "[trait]") {
	std::vector v{1, 2, 3};
	REQUIRE(IsSpecialization<decltype(v), std::vector>::value);
	REQUIRE(IsSpecialization<int, std::vector>::value == false);
}
TEST_CASE("SwapTemplate testing", "[trait]") {
	using floatVector = SwapTemplateParameter<float, std::vector<int>>::Type;
	REQUIRE(std::is_same_v<floatVector, std::vector<float>>);
}
