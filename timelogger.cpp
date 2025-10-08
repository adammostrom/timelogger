#include "timelogger.h"
#include "utils.h"
#include <filesystem>
#include <vector>
//#include "gui.h"
// #include "gui.h"

namespace fs = std::filesystem;

fs::path DATA_DIRECTORY = "log-branch/datafiles";

void confirm_directory(const std::string directory){
    if(!fs::exists(directory)){
        std::cout << "No directory found. Creating new directory... \n";
        if (fs::create_directory(directory)) {
            std::cout << "Directory created: " << directory << "\n";
        }
    }
}



std::atomic<bool> quit(false);

std::string csv_file = "work_hours.csv";


// ofstream = write data to file
// ifstream = read data from file

/* int test_gui(int argc, char **argv){
    GUI gui(250, 100, "Timer App");
    gui.button->callback([](Fl_Widget*, void*){
        std::cout << "Start clicked! " << endl;
    });
    gui.show(argc, argv);
    return Fl::run();
} */

int main(int argc, char* argv[]){


    confirm_directory(DATA_DIRECTORY);




    // Show the current time worked on start
    get_current_worked();
    
    if(argc >= 2){
        std::string cmd = argv[1];
        if (cmd == "start") start_calculator();
        else if (cmd == "end") end_calculator();
        else if (cmd == "break") break_start();
        else show_menu();
        return 0;
    }
    

    show_menu();
    return 0;

}


std::vector <std::string> menu_options = {};


void show_menu(){
    //current_log_file();
    std::cout << "\rFollowing commands available:\n " 
         //<< "> TIME TRACKING: \n"
         << "1. Start  (s) \n "
         << "2. End    (e) \n "
         << "3. Break  (b) \n "
         //<< "MANUAL ENTRY: \n"
         << "4. Manual start entry  (md) \n "
         << "5. Manual break entry  (mb) \n "
         << "6. Manual end entry    (me) \n "
         //<< "MAINTENANCE: \n"
         << "7. Clear temporary files  (cl) \n "
         << "8. Cancel                 (c)  \n "
         << "Input a command: \n ";

    int command;
    std::string input;

    std::cin >> input;


    if(input == "1" || input == "s" ){
        command = 1;
    }
    else if(input == "2" || input == "e" ){
        command = 2;
    }
    else if(input == "3" || input == "b" ){
        command = 3;
    }
    else if(input == "4" || input == "md" ){
        command = 4;
    }
    else if(input == "5" || input == "mb" ){
        command = 5;
    }
    else if(input == "6" || input == "me" ){
        command = 6;
    }
    else if(input == "7" || input == "cl" ){
        command = 7;
    }
    else if(input == "8" || input == "c" ){
        command = 8;
    }
    else command = 0;

    switch_command(command);
}

int switch_command(const int &command){
        
    switch(command) {
        case 1:
            start_calculator();
            break;
        case 2:
            end_calculator();
            break;
        case 3:
            break_start();
            break;
        case 4:
            manual_session_entry();
            break;
        case 5:
            manual_break_entry();
            break;
        case 6:
            manual_end_entry();
            break;
        case 7:
            clear_temp_files();
            break;
        case 8:
            return 0;
        default:
            std::cout << "\rCancelled or no command given.";
            break;
        }
    return 0;
}

/* void current_log_file(){
    std::cout << "Current log file in use: " << DATA_FILE << "\n";
} */


bool check_session_started(){
    std::ifstream start_state_file(".start_state.txt");

    if(!start_state_file){
        return false;
    }

    long temp = 0;

    start_state_file >> temp;
    start_state_file.close();

    if (temp == 0){

        return false;
        
    }
    return true;

}

void get_current_worked(){

    if(!check_session_started()){
        return;
    }

    long start_state = read_from_file(".start_state.txt");

    // Read from file
    std::ifstream break_time_file(".break_total.txt");

    long break_total = 0;

    if( break_time_file >> break_total){
        
    };

    long break_hours = calculate_hour_from_seconds(break_total);
    long break_minutes = calculate_mins_from_seconds(break_total);


    time_t now = get_current_time();

    // Unix gets the total seconds, the difference will be in seconds. 
    long seconds = static_cast<int>(difftime(now,start_state)) - break_total;
    long hours = calculate_hour_from_seconds(seconds);
    long minutes = calculate_mins_from_seconds(seconds);

    std::cout << "+---------------------------------------------------------------------------+ \n";
    std::cout << "\r| Started: " << std::put_time(localtime(&start_state), "%H:%M \n");
    std::cout << "\r| Total time worked (break subtracted): "  << hours << " hours and "<< minutes << " minutes." << "\n";
    std::cout << "\r| Break total: " << (break_hours < 10 ? "0" : "") << break_hours << ":" << (break_minutes < 10 ? "0" : "") <<  break_minutes << "\n";
    std::cout << "+---------------------------------------------------------------------------+\n";
}

void manual_entry(const std::string &filename){


    std::tuple<int, int> hhmm = parse_entry();

    // get today's date
    time_t now = time(nullptr);
    tm local_tm = *localtime(&now);

    // overwrite hour/minute/second
    local_tm.tm_hour = std::get<0>(hhmm);
    local_tm.tm_min = std::get<1>(hhmm);
    local_tm.tm_sec = 0;

    // convert to epoch seconds
    time_t started_time = mktime(&local_tm);

    std::ofstream start_file(filename); 

    std::string message = "The time entered is: " + std::to_string(std::get<0>(hhmm)) + ":" + std::to_string(std::get<1>(hhmm)) + ". \n";

    if(!confirm(message)){
        std::cout << "Input not saved";
        return;
    }

    save_to_file(filename, started_time);
}

void manual_session_entry(){

    if(check_session_started()){
        std::cout << "Warning. Session already logged as started. Proceeding will overwrite.\n";
    }

    manual_entry(".start_state.txt");

}

void manual_end_entry(){
    if(!check_session_started()){
        throw std::runtime_error("Session not started. Cannot end non-started session.\n");
    }

    manual_entry(".end_state.txt");
    save_to_log();
}

void manual_break_entry(){

    std::tuple <int, int> hhmm = parse_entry();

    long hh = std::get<0>(hhmm);
    long mm = std::get<1>(hhmm);


    long secs = calculate_secs_from_hour_min(hh,mm);

    long tot = read_from_file(".break_total.txt");

    tot += secs;

    std::string message = "The time entered is: " + std::to_string(hh) + ":" + std::to_string(mm) + ". \n";

    if(!confirm(message)){
        std::cout << "Break time not saved\n";
        return;
    } 
    
    save_to_file(".break_total.txt", tot);

}

std::tuple<int, int> read_epoch_secs_convert_to_hhmm(const std::string &filename){
    std::ifstream file(filename);

    time_t epoch = 0;

    file >> epoch;

    struct tm *timeinfo = localtime(&epoch); // convert to local time

    // Convert to hour and minute
    int hour = timeinfo->tm_hour;
    int minute = timeinfo->tm_min;

    return std::make_tuple(hour, minute);
}

std::tuple<int, int> parse_entry(){
    int hh = 00;
    int mm = 00;
    char colon = ':';
    std::string hhmm;
    
    std::cout << "Input with format: HH:MM \n";
    std::cin >> hhmm;

    std::stringstream ss(hhmm);
    ss >> hh >> colon >> mm;

/*     if (!(ss >> hh >> colon >> mm) || colon != ':') {
        throw runtime_error("Invalid format, expected HH:MM");
    } */
    if (hh < 0 || hh > 23) {
        throw std::runtime_error("Hour must be 0–23");
    }
    if (mm < 0 || mm > 59) {
        throw std::runtime_error("Minute must be 0–59");
    }

    return std::make_tuple(hh, mm);
}

// Read the epoch time (seconds) from a file.
long read_from_file(const std::string &filename){
    std::ifstream file(filename);

    long tot = 0;
    if(file.is_open()){
        file >> tot;
    }
    file.close();
    return tot;

}

void save_to_file(const std::string &filename, int tot){
    
    std::ofstream file(filename);
    if(!file){
        throw std::runtime_error("Could not open file for writing");
    }
    file << tot;
    file.close();
}



time_t get_current_time(){
    auto now_f = std::chrono::system_clock::now();
    time_t now_c = std::chrono::system_clock::to_time_t(now_f); // Now first

    return now_c;
}

void input_thread(){
    std::string s;

    while(std::cin >> s){
        if (s == "q"){
            quit = true;
            break;
        }
    }
}


int break_start(){

    time_t now_c = get_current_time();

    save_to_file(".break_start.txt", now_c);

    std::thread t(input_thread); 

    std::cout << "Break started. Press q to cancel the break. \n";


    int total = 0;
    while (!quit) {

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
    return 0;
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


void start_calculator(){

    if(check_session_started()){
        std::cerr << "\rSession already started! \n";
        return;
    }

    time_t now_c = get_current_time();

    save_to_file(".start_state.txt", now_c);

    std::cout << "Session started! Time recorded: " << std::put_time(localtime(&now_c), "%H:%M \n");

}

void end_calculator(){
    if(!check_session_started()){
        throw std::runtime_error("Session not started, cannot end time \n"); 
        return;
    }
    
    /* auto now = system_clock::now();
    time_t end_state = system_clock::to_time_t(now); */
    time_t now_c = get_current_time();

    save_to_file(".end_state.txt", now_c);

    save_to_log();

}

void save_to_log(){

    long break_total = read_from_file(".break_total.txt");

    long end_state = read_from_file(".end_state.txt");
    
    long start_state = read_from_file(".start_state.txt");
    
    // Work time (hours:mins)
    long elapsed = (end_state - start_state) > 0 ? (end_state - start_state) : 0;

    // Total worked time (work time - break)
    long total_work_time = (elapsed - break_total) > 0 ? (elapsed - break_total) : 0;

/* 
    ///////////////// UNDER CONSTRUCTION ////////////////////
    string note = "";
    std::std::cout << "(Optional): Add note. (c) to ignore. \n";
    getline(std::std::cin, note);
    ///////////////////////////////////////////////////

     */
    std::string datafile = file_to_log_data();
    std::cout << datafile + " selected \n";
    std::ofstream log_file(datafile, std::ios::app); // append mode

    std::string logging_record = format_record(
        start_state, 
        end_state, 
        calculate_hour_from_seconds(break_total), 
        calculate_mins_from_seconds(break_total), 
        calculate_hour_from_seconds(elapsed), 
        calculate_mins_from_seconds(elapsed), 
        calculate_hour_from_seconds(total_work_time), 
        calculate_mins_from_seconds(total_work_time)
    );



    std::string message =  "NOTE: The following data will be written and stored: " + logging_record 
        + "to: " + datafile + "\n" + "Save this record? \n";

    if(confirm(message)) {
        std::cout << "Record stored. \n";
        log_file << logging_record;
    } else {
        std::cout << "Record not stored! \n";
    }
    
    log_file.close();
    
    clear_temp_files();

    return;
}

std::string format_record(time_t start_state, time_t end_state, long  break_total_hour,
                     long break_total_mins, long worked_hours,
                     long worked_mins, long total_work_time_hours, long total_work_time_mins) 
{
    std::ostringstream oss;
    oss << std::put_time(localtime(&end_state), "%Y-%m-%d")                                       << ","  
        << std::put_time(localtime(&start_state), "%H:%M")                                        << ","  
        << std::put_time(localtime(&end_state), "%H:%M")                                          << ","  
        << break_total_hour       << ":" << std::setw(2) << std::setfill('0') << break_total_mins      << ","  
        << worked_hours           << ":" << std::setw(2) << std::setfill('0') << worked_mins           << "," 
        << total_work_time_hours  << ":" << std::setw(2) << std::setfill('0') << total_work_time_mins  << "\n";  
        //<< note << "\n";

    return oss.str();
}

void clear_file(const std::string& filename) {
    std::ofstream file(filename, std::ios::trunc); // open in truncate mode
}

void clear_temp_files(){

    std::string message = "Clear temporary files? Current data will be erased! \n";

    if(confirm(message)){
        clear_file(".break_start.txt");
        clear_file(".break_total.txt");
        clear_file(".start_state.txt");
        clear_file(".end_state.txt");

        std::cout << "Temporary files cleared! \n";

        return;
    }
    std::cout << "Temporary files not cleared! \n";
}

bool confirm(const std::string& message) {
    std::cout << message << "(yes/ENTER to accept, no to cancel): \n";

    std::string input;
    //std::cin.ignore(numeric_limits<streamsize>::max(), '\n');  
    //getline(std::cin, input);

    std::cin >> input;
    // lowercase input
    transform(input.begin(), input.end(), input.begin(), ::tolower);

    if (input == "yes" || input == "y") {
        return true; // accepted
    }
    return false; // anything else = reject
}
std::vector<std::string> read_from_directory(const std::string& path) {
    std::vector<std::string> files;
    if (path.empty())
        return files;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        files.push_back(entry.path().filename());
    }
    return files;
}

bool check_name(const std::string &name){
    int max = 30;
    int min = 5;
    if(name.size() >= 30){
        std::cout << "Too many characters. Maximum input: " + std::to_string(max) + "\n Your input: " + std::to_string(name.size()) << std::endl;
        return false;
    }
    if(name.size() < 6){ 
        std::cout << "Too few characters. Minimum input: " +  std::to_string(min) + "\n Your input: " + std::to_string(name.size()) << std::endl;        
        return false; 
    }
    return true;
}

int create_logging_file(){
    std::cout << "Please give a name for the logging file, minimum 5 characters, max 30 characters: " << std::endl; 
    std::string name;
    std::cin >> name;
    while(!check_name(name)){
        std::cout << "Try again: " << std::endl;
        std::cin >> name;
    }

    fs::path destination = fs::path(DATA_DIRECTORY) / (name + ".csv");


    confirm_directory(DATA_DIRECTORY);

    // Create the file at destination
    std::ofstream file(destination);
    if (!file) {
        std::cerr << "Failed to create file: " << destination << "\n";
        return 1;
    }

    file << "date,start,end,break_hour:min,work_hour:min,tot_hour:min \n"; 
    file.close();

    std::cout << "Created file: " << destination << "\n";

    return 0;
}


std::string file_to_log_data(){
    
    if(read_from_directory(DATA_DIRECTORY).empty()){
        std::cout << "No logging files found. Proceed to create one?\n";
        std::string choice;
        std::cin >> choice;
        if(confirm(choice)){
            create_logging_file();
        }
    }

    std::vector<std::string> datafiles = read_from_directory(DATA_DIRECTORY);

    std::cout << "Files in datadirectory for logging: \n";
    for(int i = 0; i < datafiles.size();  i++){
        std::cout << std::to_string(i) << ". " << datafiles[i] << std::endl;
    } 

    std::cout << "Select which logfile to store the data. \n";
    int input;
    std::cin >> input;

    // use filesystem::path
    fs::path fullpath = fs::path(DATA_DIRECTORY) / datafiles[input];
    return fullpath.string();  
//
}

