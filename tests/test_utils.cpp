#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "utils.hpp"

TEST_CASE("Hours are converted to seconds") {
    REQUIRE(calculate_hour_from_seconds(3600) == 1);
    REQUIRE(calculate_hour_from_seconds(7200) == 2);
    REQUIRE(calculate_hour_from_seconds(0) == 0);
}