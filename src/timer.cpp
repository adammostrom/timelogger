#include "timer.hpp"


StatusParams read_session_data(){

    StatusParams statusParams;
    
    auto start_res = read_from_file(Files::SessionStart);
    if(!start_res.ok()){
        print_log_error(start_res.error);
    }
    
    auto break_total = read_from_file(Files::BreakTotal);
    if(!break_total.ok()){
        print_log_error(break_total.error);
    }
    
    auto end_res = read_from_file(Files::SessionEnd);
    if(!end_res.ok()){
        print_log_error(end_res.error);
    }
    
    auto break_start_res = read_from_file(Files::BreakStart);
    if(!break_start_res.ok()){
        print_log_error(break_start_res.error);
    }

    statusParams.start_state   = start_res.value;
    statusParams.end_state     = end_res.value;
    statusParams.total         = (end_res.value == 0) ? 0 : end_res.value - start_res.value - break_total.value; 
    statusParams.break_start   = break_start_res.value;
    statusParams.break_total   = break_total.value;

    return statusParams;
}

std::string epoch_to_hhmm(time_t epoch) {
    std::tm tm = *std::localtime(&epoch);
    std::ostringstream out;
    out << std::put_time(&tm, "%H:%M");
    return out.str();
}


std::tuple<int, int> read_epoch_secs_convert_to_hhmm(time_t epoch){

    struct tm *timeinfo = localtime(&epoch); 

    int hour = timeinfo->tm_hour;
    int minute = timeinfo->tm_min;

    return std::make_tuple(hour, minute);
}


time_t convert_hhmm_to_epoch(long hh, long mm) {

    std::time_t now = std::time(nullptr);
    std::tm local = *std::localtime(&now);

    local.tm_hour = hh;
    local.tm_min  = mm;
    local.tm_sec  = 0;

    return std::mktime(&local);
}

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


void start_calculator() {

    auto start_res = read_from_file(Files::SessionStart);
    if(!start_res.ok()){
        print_log_error(start_res.error);
    }
    if (start_res.value > 0) {
        std::cout << "Session already started. Proceeding will overwrite current stored data! \n" ;
        if (confirm() != ConfirmResult::Yes ) return;
    }
    time_t now_c = get_current_time();

    auto res = save_to_file(Files::SessionStart, now_c);

    if(!res.ok()){
        std::cout << "Failed to save start time for: " << res.value << "\n";
        print_log_error(res.error);
    } else {
        std::cout << "Session started at: " << epoch_to_hhmm(now_c) << "\n";
    }
}

// 2026-02-11: Redesign break, abandon the timer and have it store to file like start and end state. Prompt from main loop "get_break_start" and if positive, print "on break".
// 2026-02-15: Question, do start_break actualyl need to check the break start file, if it will delete it when break has ended anyway?

void start_break(){
    auto break_res = read_from_file(Files::BreakStart);

    // Is this really needed here?
    if(!break_res.ok()){
        print_log_error(break_res.error);
    }
    if (break_res.value > 0) {
        std::cout << "Break already started \n";
        return;
    }
    time_t now_c = get_current_time();

    auto res = save_to_file(Files::BreakStart, now_c);

    if(!res.ok()){
        std::cout << "Failed to save start time for: " << res.value << "\n";
        print_log_error(res.error);
        return;
    } else {
        std::cout << "Break started at: " << epoch_to_hhmm(now_c) << "\n";
    }
}
// New design, whenever a break is started, create the file, this function
// then checks if the file exists and then checks the epoch time, and if that is ok, it
// saves the end time to file then removes the file. The main loop then checks if the file exists and if it does, it prints "on break since: " + epoch_to_hhmm(epoch time from file).
void end_break(){
    auto break_start_res = read_from_file(Files::BreakStart);

    if(!break_start_res.ok()){
        print_log_error(break_start_res.error);
        return;
    }
    if (break_start_res.value <= 0){
        std::cout << "Break not started, cannot end break time.\n";
        return;
    }

    time_t now_c = get_current_time();

    auto diff = difftime(now_c, break_start_res.value);

    // 2026-02-11 bug: if the break_total file doesnt exist, this will fail and not save the break time, even though the break time is valid. Fix by making sure to create the file with 0 if it doesnt exist when starting a session.
    auto res = save_to_file(Files::BreakTotal, (diff < 0 ? 0 : diff));

    if(!res.ok()){
        // std::cout << "Failed to save end time for: " << res.value <<"\n";
        print_log_error(res.error);
    } else {
        std::cout << "Break end time saved: " << epoch_to_hhmm(now_c) << "\n";
    }
    safe_delete_file(Files::BreakStart.c_str());
    //std::remove(Files::BreakStart.c_str());
}


/*
Problem: what if the actual epoch = 0? (midnight Jan 1, 1970). Very unlikely, but a professional design would return std::optional<time_t>.
*/
void end_calculator() {
    
    
    auto start_res = read_from_file(Files::SessionStart);
    if(!start_res.ok()){
        print_log_error(start_res.error);
        return;
    }
    if((start_res.value <= 0)){
        std::cout << "Session not started, cannot end time.\n";
        return;
    }
    
    auto end_time_res = read_from_file(Files::SessionEnd); // ideally optional<long>
    if(!end_time_res.ok()){
        print_log_error(end_time_res.error);
        return;
    }
    if (end_time_res.value != 0) {
        std::cout << "End time already logged: " << epoch_to_hhmm(end_time_res.value) << " Proceeding will overwrite, proceed? \n";
        if (confirm() != ConfirmResult::Yes) return;
    }
    time_t now_c = get_current_time();
    
    auto res = save_to_file(Files::SessionEnd, now_c);
    if(!res.ok()){
        // std::cout << "Failed to save end time for: " << res.value <<"\n";
        print_log_error(res.error);
    } else {
        std::cout << "End time saved: " << epoch_to_hhmm(now_c) << "\n";
    }
}

