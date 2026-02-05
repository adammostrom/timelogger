#include "logger.hpp"

// TODO: REFACTOR
void save_to_log(){

    // Ensure directory exists in order to store the data.
    auto dir_status = ensure_directory_exists(DATA_DIRECTORY);
    if(!dir_status.ok()){
        print_log_error(dir_status.error);  // Find a way to also emit the value
        return;
    }

    auto break_log = read_from_file(".break_total.txt");
    if(!break_log.ok()){
        print_log_error(break_log.error);
        return;
    }
    long break_total = break_log.value;

    auto end_state_log = read_from_file(".end_state.txt");
    if(!end_state_log.ok()){
        print_log_error(end_state_log.error);
        return;
    }
    long end_state = end_state_log.value;

    auto start_state_log = read_from_file(".start_state.txt");
    if(!start_state_log.ok()){
        print_log_error(start_state_log.error);
        return;
    }
    long start_state = start_state_log.value;

    
    // Work time (hours:mins)
    long elapsed = (end_state - start_state) > 0 ? (end_state - start_state) : 0;

    // Total worked time (work time - break)
    long total_work_time = (elapsed - break_total) > 0 ? (elapsed - break_total) : 0;


/*     // Important invariants!

    2026-02-05 Make a util function that checks this when "end session" or "manual end session" are called, we dont need the checks here.
    if (start_state <= 0 || end_state <= 0) {
        std::cerr << "Cannot save log: session not properly started or ended.\n";
        return;
    }   

    if (end_state < start_state) {
        std::cerr << "Invalid session: end before start.\n";
        return;
    }
 */

    // TODO: Make into separate CLI function

    std::string note;
    std::cout << "(Optional): Add note. (c to ignore)\n";

    std::getline(std::cin >> std::ws, note);
    //           ^^^^^^^^^^^^^^^^^
    // eats leading whitespace safely
    if (note == "c") {
        note.clear();
    }
        
    std::string datafile = file_to_log_data();
    std::cout << datafile + " selected \n";
    std::ofstream log_file(datafile, std::ios::app); // append mode


    LogEntry logEntry;

    logEntry.start = start_state;
    logEntry.end   = end_state;
    logEntry.break_tot_h  = calculate_hour_from_seconds(break_total);
    logEntry.break_tot_m  = calculate_mins_from_seconds(break_total);
    logEntry.worked_h     = calculate_hour_from_seconds(elapsed);
    logEntry.worked_m     = calculate_mins_from_seconds(elapsed);
    logEntry.worked_tot_h = calculate_hour_from_seconds(total_work_time);
    logEntry.worked_tot_m = calculate_mins_from_seconds(total_work_time);
    logEntry.note = note;

    std::tm start_tm = *std::localtime(&logEntry.start);
    std::tm end_tm = *std::localtime(&logEntry.end);

    std::string logging_record = format_record(logEntry, start_tm, end_tm);



    std::string message =  "NOTE: The following data will be written and stored: " + logging_record 
        + "to: " + datafile + "\n" + "Save this record? \n";

    if(confirm(message)) {
        std::cout << "Record stored. \n";
        log_file << logging_record;
    } else {
        std::cout << "Record not stored! \n";
    }
    
    log_file.close();
    
    clear_temp_files_wrapper();

    return;
}





/* std::optional<long> read_from_file_op(const std::string &filename) {
    std::ifstream file(filename);
    if (!file) return std::nullopt;

    if (file.peek() == std::ifstream::traits_type::eof()) return std::nullopt;

    long tot;
    if (!(file >> tot)) return std::nullopt;

    return tot;
} */

// Read the old value first, then add the new one.
bool save_to_file(const std::string &filename, time_t tot){
    if(!overwrite_file_int(filename, tot)){
        std::cout << "Failed to save to file: " << filename;
        return false;
    }
    else {
        std::cout << "Successfully saved data: " << tot << " to; " << filename << "\n";
        return true;
    }
}

bool overwrite_file_int(const std::string &filename, int value) {
    // Open output file in trunc mode
    std::ofstream file(filename, std::ios::trunc);
    if (!file) return false;

    file << value;
    if (!file) return false; // check write success

    return true;
}



bool clear_temp_files_operation(){
    bool ok = true;
    // Returns true only if all calls return true.
    ok &= clear_file(".break_start.txt");
    ok &= clear_file(".break_total.txt");
    ok &= clear_file(".start_state.txt");
    ok &= clear_file(".end_state.txt");
    
    return ok;
}

bool clear_file(const std::string& filename) {
    std::ofstream file(filename, std::ios::trunc); // open in truncate mode

    std::ifstream check(filename);
    check.seekg(0, std::ios::end);
    if (check.tellg() != 0) {
        return false;
    }
    return file.good();
}


// TODO: Do I really want to return filesystem::path?
Result<std::filesystem::path> create_log_file(const std::string& name){

    std::filesystem::path destination = std::filesystem::path(DATA_DIRECTORY) / (name + ".csv");

    // Create the file at destination
    std::ofstream file(destination);
    if (!file) {
        return {destination, LogError::CreateFileFailed};
    }

    file << "date,start,end,break_hour:min,work_hour:min,tot_hour:min,notes \n"; 
    file.close();

    return {destination, LogError::None}; // TODO, return destination even if successfull?
}



