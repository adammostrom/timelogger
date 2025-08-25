#include <iostream>
#include <ctime>
#include <chrono>
#include <fstream>
#include <iomanip> 
#include <string>
#include <thread>
#include <atomic>

using namespace std;
using namespace std::chrono;

int break_start();
int break_stop();
void get_current_worked();
int start_calculator();
int end_calculator();
void manual_day_entry();