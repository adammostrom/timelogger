#include "cli_input.hpp"




void manual_entry(const std::string &filename) {
    // Prompt user for HHMM input
    auto result = prompt_hhmm();
    if (!result.has_value()) {
        std::cout << "Input cancelled.\n";
        return;
    }
    time_t entered_time = result.value();

    // Read the current opposite time for validation
    long other_time = 0;
    if (filename == ".start_state.txt") {
        other_time = read_from_file(".end_state.txt");
        if (other_time > 0 && entered_time > other_time) {
            std::cout << "Start session cannot be after current end time!\n";
            return;
        }
    } else if (filename == ".end_state.txt") {
        other_time = read_from_file(".start_state.txt");
        if (other_time > 0 && entered_time < other_time) {
            std::cout << "End session time cannot predate start session time!\n";
            return;
        }
    }

    // Confirm with user before saving
    std::string message = "The time entered is: " + epoch_to_hhmm(entered_time) + ". Save time? ";
    if (!confirm(message)) {
        std::cout << "Input not saved.\n";
        return;
    }

    // Save to file
    if (save_to_file(filename, entered_time)) {
        std::cout << "Time saved: " << epoch_to_hhmm(entered_time) << "\n";
    } else {
        std::cout << "Failed to save time.\n";
    }
}

void manual_break_entry(){

    // Suggestion, ask for total time in minutes instead. Much easier
    std::cout << "Provide the break duration in minutes: \n";

    long input_mins = read_positive_integer();

    long secs = input_mins * 60;

    time_t tot = read_from_file(".break_total.txt");

    tot += secs;
    std::string message = "Break time for logging: " + std::to_string(input_mins) + " minutes. Proceed? ";

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
}



// TODO
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
}


// TODO
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

    
    std::ofstream saveTotal(".break_total.txt");
    saveTotal << total;
    std::cout << "Break period saved. \n";
    
    saveTotal.close();

    return 0;

}



// Reads from std::cin and expects an integer. 
long read_positive_integer() {
    long value = -1;

    while (true) {
        std::cin >> value;

        if (std::cin.fail() || value < 0) {
            std::cin.clear(); // clear fail flag
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