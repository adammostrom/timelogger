#pragma once

#include "data_structures.hpp"
#include "files.hpp"
#include "timer.hpp"
#include "utils.hpp"

#include <string>
#include <vector>


bool confirm_logging(const std::string &record);

bool confirm(const std::string &message);

void manual_break_entry();

void break_start();

void manual_end_entry();

void manual_session_entry();

void manual_entry(const std::string &filename);

long read_positive_integer();

Command handle_input(const std::vector<Command> &commands);
