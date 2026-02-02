#pragma once

#include "data_structures.hpp"
#include "files.hpp"
#include "timer.hpp"
#include "utils.hpp"
#include "cli_input.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <thread>
#include <algorithm>
#include <atomic>






std::atomic<bool>& quit_flag();


void clear_temp_files_wrapper();


void print_commands(const std::vector<Command> &commands);

int cancel_command();



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

bool check_name_cli(const std::string& name);

void print_log_files(const std::vector<std::string>& datafiles);

