#include "cli.hpp"




std::atomic<bool>& quit_flag() {
    static std::atomic<bool> quit{false};
    return quit;
}

// CLI wrapper: handles printing
void confirm_directory_cli(const std::string& directory) {
    if(!std::filesystem::exists(directory)) {
        std::cout << "No directory found. Creating new directory ... \n";
        if(ensure_directory_exists(directory)) {
            std::cout << "Directory created: " << directory << "\n";
        } else {
            std::cout << "Failed to create directory: " << directory << "\n";
        }
    }
}


void print_commands(const std::vector<Command>& commands){

    std::cout << "Following commands available: \n";
    
    for(int i = 0; i < commands.size() - 1; i++){
        std::cout << "\t" << "[" << std::to_string(i) << "]" << "." + commands[i].name << " (" << commands[i].command << ")" << std::endl; 
    }    
}





// Can be used instead of the checking if session started, as well as gain value in returning the actual start state (in seconds).
int get_started(){

    // Todo: Make it accept any file (for expanding the program to have several active loggers)
    std::ifstream start_state_file(Files::StartState.data());

    if(!start_state_file){
        return 0;
    }

    long temp = 0;

    start_state_file >> temp;
    start_state_file.close();

    // Return Epoch seconds logged in .start_state.txt
    return temp;
}

// TODO 2026-02-02 REFACTOR (consider stripping the statusParams of some parameters, like break. Maybe not even needing it since we can do everything here with just hhmm string outputs)
void show_status() {
    if (get_started() == 0) return;

    StatusParams statusParams = get_current_worked();

    // Read absolute times
    long start_epoch = read_from_file(".start_state.txt");
    long end_epoch   = read_from_file(".end_state.txt");

    // Determine "Logged End"
    std::string end_s = (end_epoch == 0) ? "-----" : epoch_to_hhmm(end_epoch);

    // Compute durations
    long session_duration = (start_epoch && end_epoch && end_epoch >= start_epoch) 
                            ? end_epoch - start_epoch : 0;

    long break_total = read_from_file(".break_total.txt");

    std::string break_string = duration_to_hhmm(break_total);

    // Helper function for durations
    auto duration_to_hhmm = [](long seconds) -> std::string {
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
    std::tm* tm_start = localtime(&start_epoch);

    // Print table
    std::cout << "\033[1;36m"
              << "┌──────────────────────────────────────────┐\n"
              << " * Started              : " << std::put_time(tm_start, "%H:%M") << "\n"
              << " * Session since Start  : " << statusParams.hours << "h " << statusParams.minutes << "m" << "\n"
              << " * Breaks               : " << break_string << "\n"
              << " * Logged End           : " << std::setw(5) << std::left << end_s << "\n"
              << " * End - Start diff     : " << end_start_diff << "\n"
<< "└──────────────────────────────────────────┘\033[0m\n";

    std::cout << std::setfill(' ');  // reset fill
}



void input_thread(){
    std::string s;

    while(std::cin >> s){
        if (s == "q"){
            quit_flag() = true;
            break;
        }
    }
}



// expand to be select of which files to clear
void clear_temp_files_wrapper(){
    
    std::string message = "Clear temporary files? Current data will be erased! \n";
    
    if(confirm(message)){
        clear_temp_files_operation();
        std::cout << "Temporary files cleared! \n";
    }
    else {
        std::cout << "Temporary files not cleared! \n";
    }    
    return;
}




// Pure logic: returns true if name is valid
bool is_name_valid(const std::string& name) {
    return name.size() >= 4 && name.size() <= 30;
}

// CLI wrapper: prints messages
bool check_name_cli(const std::string& name) {
    int min = 4;
    int max = 30;

    if (!is_name_valid(name)) {
        if (name.size() < min)
            std::cout << "Too few characters. Minimum input: " << min 
                      << "\nYour input: " << name.size() << "\n";
        else
            std::cout << "Too many characters. Maximum input: " << max 
                      << "\nYour input: " << name.size() << "\n";

        return false;
    }

    return true;
}

void print_log_files(const std::vector<std::string>& datafiles){
    std::cout << "Log files available: \n";
    for(int i = 1; i < datafiles.size() + 1;  i++){
            std::cout << std::to_string(i) << ". " << datafiles[i - 1] << std::endl;
        } 
    std::cout << "0. " << "Create new datafile \n";
        
}


