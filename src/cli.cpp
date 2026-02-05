#include "cli.hpp"

std::atomic<bool> &quit_flag()
{
    static std::atomic<bool> quit{false};
    return quit;
}

// CLI wrapper: handles printing
// Edit, what is the point of this? Why do the user want to know the directory is created? This is just an ensurance if the user would by accident delete the datafiles directory, so that the log data would be ensured to be able to be stored?

/* Result confirm_directory_cli(const std::string& directory) {
    if(!std::filesystem::exists(directory)) {
        std::cout << "No directory found. Creating new directory ... \n";
        if(ensure_directory_exists(directory)) {
            std::cout << "Directory created: " << directory << "\n";
            return
        } else {
            std::cout << "Failed to create directory: " << directory << "\n";
        }
    }
}
 */

void print_commands(const std::vector<Command> &commands)
{

    std::cout << "Following commands available: \n";

    for (int i = 0; i < commands.size() - 1; i++)
    {
        std::cout << "\t" << "[" << std::to_string(i) << "]" << "." + commands[i].name << " (" << commands[i].command << ")" << std::endl;
    }
}

// Can be used instead of the checking if session started, as well as gain value in returning the actual start state (in seconds).
// 2026-02-05 Do we need this fucntion? read_from_file does basically the same thing and is safer.
/* int get_started()
{

    // Todo: Make it accept any file (for expanding the program to have several active loggers)
    std::ifstream start_state_file(Files::StartState.data());

    if (!start_state_file)
    {
        return 0;
    }

    long temp = 0;

    start_state_file >> temp;
    start_state_file.close();

    // Return Epoch seconds logged in .start_state.txt
    return temp;
}
 */
// TODO 2026-02-02 REFACTOR (consider stripping the statusParams of some parameters, like break. Maybe not even needing it since we can do everything here with just hhmm string outputs)
void show_status()
{

    StatusParams statusParams = get_current_worked();

    // Read absolute times
    long start_epoch = read_from_file(".start_state.txt").value;

    if(start_epoch == 0){
        return;
    }
    long end_epoch = read_from_file(".end_state.txt");

    // Determine "Logged End"
    std::string end_s = (end_epoch == 0) ? "-----" : epoch_to_hhmm(end_epoch);

    // Compute durations
    long session_duration = (start_epoch && end_epoch && end_epoch >= start_epoch)
                                ? end_epoch - start_epoch
                                : 0;

    long break_total = read_from_file(".break_total.txt");

    std::string break_string = duration_to_hhmm(break_total);

    // Helper function for durations
    auto duration_to_hhmm = [](long seconds) -> std::string
    {
        int h = seconds / 3600;
        int m = (seconds % 3600) / 60;
        std::ostringstream oss;
        oss << std::setw(2) << std::setfill('0') << h
            << ":"
            << std::setw(2) << std::setfill('0') << m;
        return oss.str();
    };

    std::string end_start_diff = (session_duration > 0) ? duration_to_hhmm(session_duration) : "-----";

    // Format start time safely
    std::tm *tm_start = localtime(&start_epoch);

    // Print table
    std::cout << "\033[1;36m"
              << "┌──────────────────────────────────────────┐\n"
              << " * Started              : " << std::put_time(tm_start, "%H:%M") << "\n"
              << " * Session since Start  : " << statusParams.hours << "h " << statusParams.minutes << "m" << "\n"
              << " * Breaks               : " << break_string << "\n"
              << " * Logged End           : " << std::setw(5) << std::left << end_s << "\n"
              << " * End - Start diff     : " << end_start_diff << "\n"
              << "└──────────────────────────────────────────┘\033[0m\n";

    std::cout << std::setfill(' '); // reset fill
}

void input_thread()
{
    std::string s;

    while (std::cin >> s)
    {
        if (s == "q")
        {
            quit_flag() = true;
            break;
        }
    }
}

// expand to be select of which files to clear
void clear_temp_files_wrapper()
{

    std::string message = "Clear temporary files? Current data will be erased! \n";

    if (confirm(message))
    {
        clear_temp_files_operation();
        std::cout << "Temporary files cleared! \n";
    }
    else
    {
        std::cout << "Temporary files not cleared! \n";
    }
    return;
}

/* ***************** CREATE LOGGING FILE ***************** */

void create_logging_file()
{

    auto name_opt = prompt_name();

    if (!name_opt)
    {
        std::cout << "Logging file creation cancelled\n";
        return;
    }

    const std::string &name = *name_opt; // uncertainty ends here

    auto dir_status = ensure_directory_exists(DATA_DIRECTORY);
    if (!dir_status.ok())
    {
        print_log_error(dir_status.error); // Find a way to also emit the value
        return;
    }

    auto log_status = create_log_file(name);
    if (!log_status.ok())
    {
        print_log_error(log_status.error);
        return;
    }

    std::cout << "Successfully created logging file: " + name + "\n";
}

std::optional<std::string> prompt_name()
{
    std::string range = "Must be between " + std::to_string(LOG_FILE_MIN) + " and " + std::to_string(LOG_FILE_MAX);
    std::cout << "Provide a name for the logging file. " + range + "(c to cancel): \n";
    std::string name;

    while (true)
    {
        std::cin >> name;

        if (name == "c")
        {
            return std::nullopt;
        }

        if (prompt_valid_filename(name))
        {
            return name;
        }

        std::cout << range << "\n";
    }
}

// CLI wrapper: prints messages
// RENAME : prompt_valid_filename
bool prompt_valid_filename(std::string name)
{
    name.erase(std::remove(name.begin(), name.end(), ' '), name.end());
    if (name.size() >= LOG_FILE_MIN && name.size() <= LOG_FILE_MAX)
        return true;
    else
        return false;
}

/* ***************** PRINT ***************** */

// its a printer, it should only print the log files, not be smart to understand if there are log files or not.
// Just give it a vector and tell it "dont think, just list these log files".
void print_log_files(const std::vector<std::string> &datafiles)
{
    std::cout << "Log files available: \n";
    for (int i = 1; i < datafiles.size() + 1; i++)
    {
        std::cout << std::to_string(i) << ". " << datafiles[i - 1] << std::endl;
    }
    //std::cout << "0. " << "Create new datafile \n";
}

// Returns the filepath for the file to store the data.
// TODO: Refactor to return Result
std::optional<std::filesystem::path> file_to_log_data(){

    int input;

    auto dir_log = ensure_directory_exists(DATA_DIRECTORY);
    if (!dir_log.ok())
    {
        print_log_error(dir_log.error);
        return std::nullopt;
    }

    auto datafiles = read_from_directory(DATA_DIRECTORY);
    if (datafiles.empty()) {
        std::cout << "No log files available.\n";
        return std::nullopt;
    }

    while (true)
    {
        std::cout << "Select a file to store the data: (0 to cancel) \n";

        print_log_files(datafiles);

        // Makes sure we read a positive integer
        size_t input = read_positive_integer();

        if (input == 0){
            return std::nullopt;
        }

        if (input > datafiles.size())
        {
            std::cout << "Invalid option. Try again.\n";
            continue;
        }
        return std::filesystem::path(DATA_DIRECTORY) / datafiles[input - 1];
    }
}



void manual_entry(const std::string &filename)
{
    // Prompt user for HHMM input
    auto result = prompt_hhmm();
    if (!result.has_value())
    {
        std::cout << "Input cancelled.\n";
        return;
    }
    time_t entered_time = result.value();

    // Read the current opposite time for validation
    long other_time = 0;
    if (filename == ".start_state.txt")
    {
        other_time = read_from_file(".end_state.txt").value;
        if (other_time > 0 && entered_time > other_time)
        {
            std::cout << "Start session cannot be after current end time!\n";
            return;
        }
    }
    else if (filename == ".end_state.txt")
    {
        other_time = read_from_file(".start_state.txt").value;
        if (other_time > 0 && entered_time < other_time)
        {
            std::cout << "End session time cannot predate start session time!\n";
            return;
        }
    }

    // Confirm with user before saving
    std::string message = "The time entered is: " + epoch_to_hhmm(entered_time) + ". Save time? ";
    if (!confirm(message))
    {
        std::cout << "Input not saved.\n";
        return;
    }

    // Save to file
    if (save_to_file(filename, entered_time))
    {
        std::cout << "Time saved: " << epoch_to_hhmm(entered_time) << "\n";
    }
    else
    {
        std::cout << "Failed to save time.\n";
    }
}

void manual_break_entry()
{

    // Suggestion, ask for total time in minutes instead. Much easier
    std::cout << "Provide the break duration in minutes: \n";

    long input_mins = read_positive_integer();

    long secs = input_mins * 60;

    time_t tot = read_from_file(".break_total.txt").value;

    tot += secs;
    std::cout << "Break time for logging: " << std::to_string(input_mins) <<  " minutes. Proceed? ";

    if (confirm() == ConfirmResult::Yes)
    {
        save_to_file(".break_total.txt", tot);
        return;
    }
    std::cout << "Break time not saved\n";
    return;

}

void manual_session_entry(){

    if (read_from_file(".start_state.txt").value > 0)
    {
        std::cout <<  "Warning. Session already logged as started. Proceeding will overwrite.";
        if (confirm() == ConfirmResult::Yes)
        {
            manual_entry(".start_state.txt");
            return;
        }
    }
    std::cout << "Cancelled.\n";
    return;
}

void manual_end_entry()
{
    if (get_started() < 1)
    {
        throw std::runtime_error("Session not started. Cannot end non-started session.\n");
    }

    manual_entry(".end_state.txt");
}

// 2026-02-05: Refactored (it handles a lot of subfunctions, but it doesnt compute anything nor does it make any decisions)
// is a use-case function (also called an orchestration or application-service function).
// it does many things — but it does not decide many things.
void break_start()
{

    time_t now_c = get_current_time();

    std::cout << "Break started. Press q to cancel the break. \n";

    long elapsed = start_timer(now_c);

    // Unix gets the total seconds, the difference will be in seconds. Divide by 60 and we get minutes
    long seconds = static_cast<int>(difftime(now_c, elapsed));
    long hours   = calculate_hour_from_seconds(seconds);
    long minutes = calculate_mins_from_seconds(seconds);

    auto break_tot_log = read_from_file(".break_total.txt");
    if(!break_tot_log.ok()){
        print_log_error(break_tot_log.error);
        return;
    }

    int remains = seconds % 60;

    std::cout << "\nBreak ended. Summary: " 
              << hours << std::setw(2) << std::setfill('0') << " hours, "
              << minutes << std::setw(2) << std::setfill('0') << " minutes and "
              << remains << " seconds. Save break period? (or e for edit) ";

    std::string command;
    std::cin >> command;

    if (command == "e")
    {
        manual_break_entry();
    }
    
    if (confirm() != ConfirmResult::Yes){
        std::cout << "Break period not saved. \n";
    }

    long total = break_tot_log.value + elapsed;
    save_to_file(".break_total.txt", total);

    std::cout << "Break period saved.\n";
}

// Runs a timer and stops when user cancels with "q", otherwise returns the elapsed.
long start_timer(time_t now_c){

    std::thread t(input_thread);

    while (!quit_flag())
    {
        time_t elapsed = time(nullptr) - now_c; // seconds since break started
        
        long hours   = calculate_hour_from_seconds(elapsed);
        long minutes = calculate_mins_from_seconds(elapsed);
        long seconds = elapsed % 60;

        std::cout << "\rOn break: "
                  << hours << ":" 
                  << (minutes < 10 ? "0" : "") << minutes << ":"
                  << (seconds < 10 ? "0" : "") << seconds << "   >" 
                  << std::flush;

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    t.join();

    return time(nullptr) - now_c;
}


// Reads from std::cin and expects an integer.
long read_positive_integer()
{
    long value = -1;

    while (true)
    {
        std::cin >> value;

        if (std::cin.fail() || value < 0)
        {
            std::cin.clear();                                                   // clear fail flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard bad input
            std::cout << "Invalid input. Please enter a positive integer: ";
            value = -1;
            continue;
        }

        // good input
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear rest of line
        return value;
    }
}

enum class ConfirmResult {
    Yes,
    No,
    Cancel
};

ConfirmResult confirm()
{
    while (true) {
        std::cout << " (y/n, c to cancel)\n";

        std::string input;
        if (!(std::cin >> input))
            return ConfirmResult::Cancel;

        std::transform(input.begin(), input.end(), input.begin(),
            [](unsigned char c) { return std::tolower(c); });

        if (input == "y" || input == "yes") return ConfirmResult::Yes;
        if (input == "n" || input == "no")  return ConfirmResult::No;
        if (input == "c")                   return ConfirmResult::Cancel;

        std::cout << "Invalid input.\n";
    }
}




// Fine for now. Maybe refactor later.
Command handle_input(const std::vector<Command> &commands)
{

    while (true)
    {
        std::cout << "> Enter command (q to quit): ";
        std::string input;
        std::cin >> input;

        if (input == "q")
        {
            return commands[commands.size() - 1]; // Hardcoded for now
        }

        bool is_number = !input.empty() &&
                         std::all_of(input.begin(), input.end(), [](char c)
                                     { return std::isdigit(static_cast<unsigned char>(c)); });

        if (is_number)
        {
            int index = std::stoi(input);

            if (index >= 0 && index <= commands.size())
            {
                return commands[index];
            }
        }
        else
        {
            for (const Command &c : commands)
            {
                if (input == c.command)
                {
                    return c;
                }
            }
        }
        std::cout << "Unknown command. \n";
    }
}

