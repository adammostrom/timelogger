#pragma once
#include <string>
#include <tuple>
#include <ctime>
#include <chrono>

#include "data_structures.hpp"
#include "logger.hpp"
#include "utils.hpp"
#include "cli.hpp"
#include "files.hpp"

time_t convert_hhmm_to_epoch(long hh, long mm);

void start_calculator();

void end_calculator();

void break_start();

int break_stop();

StatusParams get_current_worked();

time_t get_current_time();

// 2026-02-02: This function is not being called anywhere. Suggest to remove.
std::tuple<int, int> read_epoch_secs_convert_to_hhmm(const std::string &filename);

std::string epoch_to_hhmm(time_t epoch);

std::string duration_to_hhmm(long seconds);