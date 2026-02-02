#pragma once

#include "data_structures.hpp"
#include "logger.hpp"
#include "timer.hpp"

#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include <thread>
#include <optional>

std::string format_record(const LogEntry& entry, const std::tm& start_tm, const std::tm& end_tm);

long calculate_hour_from_seconds(long seconds);
long calculate_mins_from_seconds(long seconds);
long calculate_secs_from_hour_min(long hour, long min);
long calculate_secs_from_minutes(long minutes);


bool is_manual_format_ok(int hh, int mm);

std::vector<std::string> read_from_directory(const std::string &path);

std::optional<time_t> prompt_hhmm();

bool ensure_directory_exists(const std::string& directory);

bool valid_string_hhmm(std::string_view s);

bool parse_hhmm_helper(int hh, int mm);