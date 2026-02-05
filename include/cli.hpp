#pragma once

#include "data_structures.hpp"
#include "files.hpp"
#include "timer.hpp"
#include "utils.hpp"
#include "error_handling.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <thread>
#include <algorithm>
#include <atomic>
#include <optional>



#define LOG_FILE_MIN 4
#define LOG_FILE_MAX 20


std::atomic<bool>& quit_flag();


void clear_temp_files_wrapper();


void print_commands(const std::vector<Command> &commands);

void create_logging_file();

//// WORK IN PROGRESS ////

void logged_data_overview();

// void current_log_file();

int get_started();

bool check_session_started();

void show_status();

int switch_command(const int &command);

// Directory
void confirm_directory_cli(const std::string& directory);

void input_thread();

bool prompt_valid_filename(std::string name)

std::optional<std::string> prompt_name();

void print_log_files(const std::vector<std::string>& datafiles);

bool confirm_logging(const std::string &record);

bool confirm(const std::string &message);

void manual_break_entry();

void break_start();

void manual_end_entry();

void manual_session_entry();

void manual_entry(const std::string &filename);

long read_positive_integer();

Command handle_input(const std::vector<Command> &commands);

std::optional<std::filesystem::path> file_to_log_data()