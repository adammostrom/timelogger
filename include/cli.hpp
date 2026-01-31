#pragma once

#include "data_structures.hpp"
#include "files.hpp"
#include "timer.hpp"
#include "utils.hpp"

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

bool confirm_logging(const std::string &record);

bool confirm(const std::string &message);

void print_main_menu(const std::vector<Command> &commands);
Command handle_input(const std::vector<Command> &commands);

int cancel_command();

void manual_break_entry();

void break_start();

void manual_end_entry();

void manual_session_entry();

void manual_entry(const std::string &filename);

//// WORK IN PROGRESS ////

void logged_data_overview();

// void current_log_file();

int get_started();

bool check_session_started();

void show_status(const long &start_state, int work_h, int work_m, int break_h, int break_m);

int switch_command(const int &command);

// Directory
void confirm_directory_cli(const std::string& directory);

void input_thread();

bool check_name_cli(const std::string& name);

void print_log_files(const std::vector<std::string>& datafiles);