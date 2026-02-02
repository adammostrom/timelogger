#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "utils.hpp"

TEST_CASE("Seconds are converted to hours") {
    REQUIRE(calculate_hour_from_seconds(3600) == 1);
    REQUIRE(calculate_hour_from_seconds(7200) == 2);
    REQUIRE(calculate_hour_from_seconds(0) == 0);
}



// calculate_mins_from_seconds on a given hour. 
TEST_CASE("Calculate minutes from seconds"){
    REQUIRE(calculate_mins_from_seconds(3660) == 1);
    REQUIRE(calculate_mins_from_seconds(7320) == 2);
    REQUIRE(calculate_mins_from_seconds(0) == 0);
}


// calculate_secs_from_hour_min
TEST_CASE("Calculate seconds from hour and minutes"){
    REQUIRE(calculate_secs_from_hour_min(1, 0)  == 3600);
    REQUIRE(calculate_secs_from_hour_min(1, 60) == 7200);
    REQUIRE(calculate_secs_from_hour_min(0, 0)  == 0);
    REQUIRE(calculate_secs_from_hour_min(0, 60) == 3600);
    REQUIRE(calculate_secs_from_hour_min(2, 25) == 8700);
}


// bool parse_hhmm(std::string_view s, int &hh, int &mm)


// Takes a string of hhmm format, parses it to see if it is appropriate, if yes, writes the hh and mm to the arguments.
TEST_CASE("Parse format hhmm if its accepted by numerics only, and appropriate sizes."){
    REQUIRE(valid_string_hhmm("1630") == true);
    REQUIRE(valid_string_hhmm("aabb") == false);
    REQUIRE(valid_string_hhmm("-123") == false);
    REQUIRE(valid_string_hhmm("123j") == false);
    REQUIRE(valid_string_hhmm("1234") == true);
    REQUIRE(valid_string_hhmm("0123") == true);
    REQUIRE(valid_string_hhmm("999999999") == false);
}


// parse_hhmm_helper
TEST_CASE("Check the helper for filtering out large or negative numbers (hh < 25 and mm < 61)"){
    REQUIRE(parse_hhmm_helper(16,32) == true);
    REQUIRE(parse_hhmm_helper(99,99) == false);
    REQUIRE(parse_hhmm_helper(-1,-1) == false);
    REQUIRE(parse_hhmm_helper(0,0) == true);
    REQUIRE(parse_hhmm_helper(12030,-2) == false);
    REQUIRE(parse_hhmm_helper(12,99) == false);
    REQUIRE(parse_hhmm_helper(25,61) == false);
}