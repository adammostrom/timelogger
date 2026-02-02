#include "timer.hpp"



StatusParams get_current_worked(){

    StatusParams statusParams;
    
    long start_state = get_started();

/*     if(start_state == 0){
        return;
    } */

    long break_total = read_from_file(Files::BreakTotal.data());

    long break_hours = calculate_hour_from_seconds(break_total);
    long break_minutes = calculate_mins_from_seconds(break_total);
    
    time_t now = get_current_time();
    long seconds = static_cast<int>(difftime(now,start_state)) - break_total;

    if (seconds < 0) seconds = 0;

    long hours = calculate_hour_from_seconds(seconds);
    long minutes = calculate_mins_from_seconds(seconds);

    statusParams.start_state = start_state;
    statusParams.hours = hours;
    statusParams.minutes = minutes;
    statusParams.break_hours = break_hours;
    statusParams.break_minutes = break_minutes;

    return statusParams;
}

// Returns the string printout of epoch seconds
std::string epoch_to_hhmm(time_t epoch) {
    std::tm tm = *std::localtime(&epoch);
    std::ostringstream out;
    out << std::put_time(&tm, "%H:%M");
    return out.str();
}


std::tuple<int, int> read_epoch_secs_convert_to_hhmm(time_t epoch){

    struct tm *timeinfo = localtime(&epoch); // convert to local time

    // Convert to hour and minute
    int hour = timeinfo->tm_hour;
    int minute = timeinfo->tm_min;

    return std::make_tuple(hour, minute);
}

// Convert given hour and minute into epoch
time_t convert_hhmm_to_epoch(long hh, long mm) {

    // Get today's date
    std::time_t now = std::time(nullptr);
    std::tm local = *std::localtime(&now);

    // Overwrite only the hour/minute
    local.tm_hour = hh;
    local.tm_min  = mm;
    local.tm_sec  = 0;

    // Convert back to epoch
    return std::mktime(&local);
}

// Return the duration from epoch into format HH:MM
std::string duration_to_hhmm(long seconds) {
    int h = seconds / 3600;
    int m = (seconds % 3600) / 60;
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << h
        << ":"
        << std::setw(2) << std::setfill('0') << m;
    return oss.str();
}


time_t get_current_time(){
    auto now_f = std::chrono::system_clock::now();
    time_t now_c = std::chrono::system_clock::to_time_t(now_f); // Now first

    return now_c;
}



// --- helpers ---
bool is_session_started() {
    return get_started() > 0;
}

// Save timestamp safely, return success
bool save_timestamp(const std::string &filename, time_t ts) {
    return save_to_file(filename, ts); 
}

void start_calculator() {
    if (is_session_started()) {
        std::cout << "Session already started.\n";
        if (!confirm("Proceeding will overwrite, proceed? ")) return;
    }

    time_t now_c = get_current_time();
    if (!save_timestamp(".start_state.txt", now_c)) {
        std::cerr << "Failed to save start time!\n";
    } else {
        std::cout << "Session started at: " << epoch_to_hhmm(now_c) << "\n";
    }
}


/*
Problem: what if the actual epoch = 0? (midnight Jan 1, 1970). Very unlikely, but a professional design would return std::optional<time_t>.
*/
void end_calculator() {
    if (!is_session_started()) {
        std::cout << "Session not started, cannot end time.\n";
        return;
    }

    auto end_time = read_from_file(".end_state.txt"); // ideally optional<long>

    time_t now_c = get_current_time();

    if (end_time != 0) {
        std::cout << "End time already logged: " << epoch_to_hhmm(end_time) << "\n";
        if (!confirm("Proceeding will overwrite, proceed? ")) return;
    }

    if (!save_timestamp(".end_state.txt", now_c)) {
        std::cerr << "Failed to save end time!\n";
    } else {
        std::cout << "End time saved: " << epoch_to_hhmm(now_c) << "\n";
    }
}

