#pragma once

#include "data_structures.hpp"
#include "files.hpp"
#include "cli.hpp"

#include <string>
#include <filesystem>
#include <fstream>
#include <optional>


void create_logging_file();

void save_to_log();

bool save_to_file(const std::string &filename, time_t tot);

long read_from_file(const std::string &filename);

std::string file_to_log_data();

bool clear_file(const std::string &filename);

bool clear_temp_files_operation();

bool overwrite_file_int(const std::string &filename, int value);

std::optional<long> read_from_file_op(const std::string &filename);