#include "Traits.hpp"
#include "catch2/catch.hpp"
#include <vector>
using namespace Functional;
TEST_CASE("IsSpecialization testing", "[trait]") {
	std::vector v{1, 2, 3};
	REQUIRE(IsSpecialization<decltype(v), std::vector>::value);
	REQUIRE(IsSpecialization<int, std::vector>::value == false);
}