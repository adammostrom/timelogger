#include "logger.hpp"

// TODO: REFACTOR
// TODO 2026-02-11 : add helper functions that store backups to prevent erasing data: 

/*
std::filesystem::copy_file(path, path.string() + ".bak",
                           std::filesystem::copy_options::overwrite_existing);

*/
void save_to_log(){

    // Ensure directory exists in order to store the data.
    auto dir_status = ensure_directory_exists(DATA_DIRECTORY);
    if(!dir_status.ok()){
        print_log_error(dir_status.error);  // Find a way to also emit the value
        return;
    }

    auto break_log = read_from_file(Files::BreakTotal);
    if(!break_log.ok()){
        print_log_error(break_log.error);
        return;
    }
    long break_total = break_log.value;

    auto end_state_log = read_from_file(Files::SessionEnd);
    if(!end_state_log.ok()){
        print_log_error(end_state_log.error);
        return;
    }
    long end_state = end_state_log.value;

    auto start_state_log = read_from_file(Files::SessionStart);
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
        
    std::optional<std::filesystem::path> datafile = file_to_log_data();

    std::cout << datafile.value().string() + " selected \n";
    std::ofstream log_file(datafile.value().string(), std::ios::app); // append mode


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



    std::cout <<   "NOTE: The following data will be written and stored: " << logging_record 
        << "to: " << datafile.value().string() + "\n" + "Save this record? \n";

    if(confirm()==ConfirmResult::Yes) {
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
Result<std::string> save_to_file(const std::string &filename, time_t tot){
    
    auto content = read_from_file(filename);
    if(!content.ok()){
        print_log_error(content.error);
        return {filename, LogError::SaveToFileFailed};
    }

    tot = tot + content.value;

    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()){
        return {filename, LogError::IoError};
    }

    file << tot;

    if (!file.good()){
        return {filename, LogError::IoError};
    }

    return {filename, LogError::None};
}



// Tries to remove a file safely. Returns true if file didn't exist or was removed successfully.
bool safe_delete_file(const std::filesystem::path& filepath) {
    if (!std::filesystem::exists(filepath)) {
        return true; // nothing to delete, consider success
    }

    std::error_code ec;
    std::filesystem::remove(filepath, ec);

    if (ec) {
        std::cerr << "Failed to delete file: " << filepath << " (" << ec.message() << ")\n";
        return false;
    }

    return !std::filesystem::exists(filepath); // confirm deletion
}

// Deletes all temp files and reports errors individually
bool clear_temp_files_operation() {
    bool ok = true;

    ok &= safe_delete_file(Files::BreakStart);
    ok &= safe_delete_file(Files::BreakTotal);
    ok &= safe_delete_file(Files::SessionStart);
    ok &= safe_delete_file(Files::SessionEnd);
    ok &= safe_delete_file(Files::SessionTotal);

    return ok;
}

// TODO: Do I really want to return filesystem::path?
Result<std::filesystem::path> create_log_file(const std::string& name){

    std::filesystem::path destination = std::filesystem::path(DATA_DIRECTORY) / (name + ".csv");

    // Create the file at destination
    std::ofstream file(destination);
    if (!file) {
        return {destination, LogError::CreateFileFailed};
    }

    // Todo 2026-02-11:  Fetch from datastructure.
    file << "date,start,end,break_hour:min,work_hour:min,tot_hour:min,notes \n"; 
    file.close();

    return {destination, LogError::None}; // TODO, return destination even if successfull?
}



