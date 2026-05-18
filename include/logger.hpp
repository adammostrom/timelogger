#pragma once

#include "data_structures.hpp"
#include "files.hpp"
#include "cli.hpp"
#include "error_handling.hpp"

#include <string>
#include <filesystem>
#include <fstream>
#include <optional>

Result<std::filesystem::path> create_log_file(const std::string &name);

void save_to_log();

Result<std::string> save_to_file(const std::string &filename, time_t tot);
Result<std::string> overwrite_save_to_file(const std::string &filename, time_t tot);

bool clear_file(const std::string &filename);

bool clear_temp_files_operation();

std::optional<long> read_from_file_op(const std::string &filename);

bool safe_delete_file(const std::filesystem::path &filepath);

Result<std::filesystem::path> append_csv(const std::filesystem::path &path, const std::string &content);

std::string prompt_note();