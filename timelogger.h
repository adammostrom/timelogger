#include <iostream>
#include <ctime>
#include <chrono>
#include <fstream>
#include <iomanip> 
#include <string>
#include <thread>
#include <atomic>
#include <algorithm>
#include <filesystem>
#include <limits> // for numeric_limits<streamsize>::max()
#include <tuple> // Required for std::tuple
#include <vector>

using namespace std;
using namespace std::chrono;

int break_start();
int break_stop();
void get_current_worked();
time_t get_current_time();
void start_calculator();
void end_calculator();
void manual_session_entry();
void show_menu();
//void current_log_file();


string file_to_log_data();

vector<string> read_from_directory(const string& path);

string format_record(time_t start_state, time_t end_state, long  break_total_hour,
                     long break_total_mins, long worked_hours,
                     long worked_mins, long total_work_time_hours, long total_work_time_mins);
bool confirm_logging(const string& record);
void clear_temp_files();
bool check_session_started();
bool confirm(const string& message);

int switch_command(const int &command);


void manual_break_entry();
void clear_file(const string& filename);

bool is_manual_format_ok(int hh, int mm);


tuple<int, int> parse_entry();
void manual_end_entry();


long read_from_file(const string &filename);
void save_to_log();

void save_to_file(const string &filename, int tot);

tuple<int, int> read_epoch_secs_convert_to_hhmm(const string &filename);

// WIP

tuple<time_t, time_t> get_started_time();
// GUI

int test_gui(int argc, char **argv);

void manual_entry(const string &filename);