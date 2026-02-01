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

void start_calculator();
void end_calculator();

void break_start();
int break_stop();

StatusParams get_current_worked();

time_t get_current_time();

std::tuple<int, int> read_epoch_secs_convert_to_hhmm(const std::string &filename);

std::string epoch_to_hhmm(time_t epoch);

int convert_hhmm_to_epoch(int& hh, int& mm);