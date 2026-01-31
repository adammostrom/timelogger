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


void print_main_menu(const std::vector<Command>& commands){

    std::cout << "Following commands available: \n";
    
    for(int i = 0; i < commands.size() - 1; i++){
        std::cout << "\t" << "[" << std::to_string(i) << "]" << "." + commands[i].name << " (" << commands[i].command << ")" << std::endl; 
    }    
}


// For state patch -> make this return a command to be called on in "OpState", and have the handler return the OpState

Command handle_input(const std::vector<Command>& commands){

    while(true){
        std::cout << "> Enter command (q to quit): ";
        std::string input;
        std::cin >> input;
       
        if(input == "q"){
            return commands[commands.size() - 1]; // Hardcoded for now
        }
        
        bool is_number = !input.empty() && 
        std::all_of(input.begin(), input.end(), [](char c) {return std::isdigit(static_cast<unsigned char>(c)); });
        
        if(is_number){
            int index = std::stoi(input);
    
            if(index >= 0 && index <= commands.size()){
                return commands[index];    
            }
        } else {
            for (const Command& c : commands){
                if( input == c.command){ return c;
                }
            }
        }
    std::cout << "Unknown command. \n";
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

void show_status(const long &start_state, int work_h, int work_m,
                 int break_h, int break_m) 
{
    std::tm* tm_start = localtime(&start_state);

    std::cout << "\033[1;36m"
              << "┌──────────────────────────────────────────┐\n"
              << " * Started : " << std::put_time(tm_start, "%H:%M")
              << "                           \n"
              << " * Session : " << work_h << "h " << work_m << "m"
              << std::string(30 - (std::to_string(work_h).size() +
                                   std::to_string(work_m).size()), ' ')
              << "\n"
              << " * Breaks  : "
              << std::setw(2) << std::setfill('0') << break_h << ":"
              << std::setw(2) << std::setfill('0') << break_m
              << "\n"
              << "└──────────────────────────────────────────┘\033[0m\n";

    std::cout << std::setfill(' ');  // reset fill
}



void manual_entry(const std::string &filename){

    auto result = prompt_hhmm();
    if (!result.has_value()) {
        std::cout << "Invalid input.\n";
        return;
    }
    time_t started_time = result.value();

    // get today's date
    time_t now = time(nullptr);
    tm local_tm = *localtime(&now);

/*     // overwrite hour/minute/second
    local_tm.tm_hour = std::get<0>(hhmm);
    local_tm.tm_min = std::get<1>(hhmm);
    local_tm.tm_sec = 0;

    // convert to epoch seconds
    time_t started_time = mktime(&local_tm); */

    
    std::ofstream start_file(filename); 

    std::string message = std::string("The time entered is: ") + epoch_to_hhmm(started_time) + ". Save time registered? ";

    if(!confirm(message)){
        std::cout << "Input not saved";
        return;
    }

    save_to_file(filename, started_time);
}

void manual_break_entry(){

    auto result = prompt_hhmm();
    if (!result.has_value()) {
        std::cout << "Invalid input.\n";
        return;
    }
    time_t secs = result.value();

    time_t tot = read_from_file(".break_total.txt");

    tot += secs;
                                                    
    std::string message = std::string("The time entered is: ") + epoch_to_hhmm(tot) + ". Save time registered? ";

    if(!confirm(message)){
        std::cout << "Break time not saved\n";
        return;
    } 
    
    save_to_file(".break_total.txt", tot);

}


void manual_session_entry(){

    if(get_started() > 0){
        if(!confirm("Warning. Session already logged as started. Proceeding will overwrite.")){
            std::cout << "Cancelled.\n";
            return;
        }
    }
    manual_entry(".start_state.txt");
}

void manual_end_entry(){
    if(get_started() < 1){
        throw std::runtime_error("Session not started. Cannot end non-started session.\n");
    }

    manual_entry(".end_state.txt");
    save_to_log();
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


void break_start(){

    time_t now_c = get_current_time();

    save_to_file(".break_start.txt", now_c);

    std::thread t(input_thread); 

    std::cout << "Break started. Press q to cancel the break. \n";


    int total = 0;
    while (!quit_flag()) {

        time_t elapsed = time(nullptr) - now_c;  // seconds since break started
        long hours = calculate_hour_from_seconds(elapsed);
        int minutes = calculate_mins_from_seconds(elapsed);
        int seconds = elapsed % 60;

        std::cout << "\rOn break: " << hours << ":" << (minutes < 10 ? "0" : "") << minutes << ":" 
        << (seconds < 10 ? "0" : "") << seconds << "   >" << std::flush;

        total++;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    }
    t.join();

    break_stop();
    //return 0;
}

int break_stop(){
/*     ifstream break_start_file(".break_start.txt");

    time_t start;
    break_start_file >> start;
    break_start_file.close(); */

    long start = read_from_file(".break_start.txt");

    time_t now_c = get_current_time();

    // Unix gets the total seconds, the difference will be in seconds. Divide by 60 and we get minutes 
    long seconds = static_cast<int>(difftime(now_c,start));
    long hours = calculate_hour_from_seconds(seconds);
    long minutes = calculate_mins_from_seconds(seconds);

    // Add to the total
    int total = 0;
    std::ifstream totalFile(".break_total.txt");
    if(totalFile) {
        // If we already have a file, assume it has break already logged, read to total
        totalFile >> total;
    }
    totalFile.close();

    total += seconds;
    int remains = seconds % 60;

    

    std::cout << "Break ended. Summary: " << hours << std::setw(2) << std::setfill('0') << " hours, " 
                                     << minutes << std::setw(2) << std::setfill('0') << " minutes and " 
                                     << remains << " seconds. \nSave break period? (yes/no) \n"
                                     << "or e for edit \n";

    
    std::string command;
    std::cin >> command;

    if(command == "e"){
        manual_break_entry();
    }

    if(command == "no"){
        std::cout << "Break period not saved. \n";
        return 0;
    }

    std::cout << "Break period saved. \n";

    std::ofstream saveTotal(".break_total.txt");
    saveTotal << total;
    saveTotal.close();

    return 0;

}


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


bool confirm(const std::string& message) {

    while(true){
        std::cout << message << "(y/n, c to cancel) \n";
    
        std::string input;
        //std::cin.ignore(numeric_limits<streamsize>::max(), '\n');  
        //getline(std::cin, input);
    
        std::cin >> input;
        // lowercase input
        std::transform(input.begin(), input.end(), input.begin(), ::tolower);
    
        if (input == "yes" || input == "y") {
            return true; // accepted
        }
        if(input == "c" || input == "n"){
            return false;
        }
        return false; // anything else = reject

    }
    std::cout << "Invalid input.\n";
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
