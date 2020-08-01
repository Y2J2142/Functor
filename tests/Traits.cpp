#include "Traits.hpp"
#include "catch2/catch.hpp"
#include <vector>
using namespace Functor;
TEST_CASE("Traits testing", "[trait]") {
	std::vector v{1, 2, 3};
	REQUIRE(is_specialization<decltype(v), std::vector>::value);
	REQUIRE(!is_specialization<int, std::vector>::value);
}