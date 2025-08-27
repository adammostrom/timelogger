#include <iostream>
#include <ctime>
#include <chrono>
#include <fstream>
#include <iomanip> 
#include <string>
#include <thread>
#include <atomic>
#include <algorithm>
#include <limits> // for numeric_limits<streamsize>::max()

using namespace std;
using namespace std::chrono;

int break_start();
int break_stop();
void get_current_worked();
int start_calculator();
int end_calculator();
void manual_day_entry();

long calculate_hour_from_seconds(long seconds);
long calculate_mins_from_seconds(long seconds);
string format_record(time_t start_state, time_t end_state, long  break_total_hour,
                     long break_total_mins, long worked_hours,
                     long worked_mins, long total_work_time_hours, long total_work_time_mins);
bool confirm_logging(const string& record);
void clear_temp_files();
bool check_day_started();
bool confirm(const string& message);


// GUI

int test_gui(int argc, char **argv);