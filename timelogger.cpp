#include "timelogger.h"
#include "utils.h"

//#include "gui.h"
// #include "gui.h"


atomic<bool> quit(false);

string csv_file = "logged_times.csv";
string csv_testfile = "TEST_logged_times.csv";

string active_log_file = csv_file;

//#define TESTING   // uncomment this when testing


/* #if defined(TESTING)
constexpr char DATA_FILE[] = "TEST_logged_times.csv";
#elif defined(STUDY)
constexpr char DATA_FILE[] = ".study_hours.csv";
#else
constexpr char DATA_FILE[] = "logged_times.csv";
#endif */

string DATA_FILE = "logged_times.csv";
//string DATA_FILE = ".study_hours.csv";
// string DATA_FILE = "TEST_logged_times.csv";


// ofstream = write data to file
// ifstream = read data from file

/* int test_gui(int argc, char **argv){
    GUI gui(250, 100, "Timer App");
    gui.button->callback([](Fl_Widget*, void*){
        cout << "Start clicked! " << endl;
    });
    gui.show(argc, argv);
    return Fl::run();
} */

int main(){

    // Show the current time worked on start
    if(check_day_started()){
        get_current_worked();
    }

    int command;
    char input;
    cout << "\rFollowing commands available:\n " 
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
    cin >> input;

    if(input == '1' || input == 's' ){
        command = 1;
    }
    if(input == '2' || input == 'e' ){
        command = 2;
    }
    if(input == '3' || input == 'b' ){
        command = 3;
    }
    if(input == '4' || input == 'md' ){
        command = 4;
    }
    if(input == '5' || input == 'mb' ){
        command = 5;
    }
    if(input == '6' || input == 'me' ){
        command = 6;
    }
    if(input == '7' || input == 'cl' ){
        command = 7;
    }
    if(input == '8' || input == 'c' ){
        command = 8;
    }

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
            manual_day_entry();
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
            cout << "\rCancelled or no command given.";
            break;
        }
}


bool check_day_started(){
    ifstream start_state_file(".start_state.txt");

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
/* 
tuple <time_t, time_t> get_started_time(){
    ifstream start_time_file(".start_state.txt");
    tuple<time_t, time_t> day_started;
    if(start_time_file){
        time_t start_time = 0;
        time_t start_state = 0;

        start_time_file >> start_state;
        start_time_file >> start_time;

        start_time_file.close();
        
        auto day_started = make_tuple(start_state, start_time);
        return day_started;
    }
    return
} */

void get_current_worked(){

    if(!check_day_started()){
        cout << "Day not started" << endl;
        return;
    }

    ifstream start_time_file(".start_state.txt");

    time_t start_time = 0;
    time_t start_state = 0;

    start_time_file >> start_state;
    start_time_file >> start_time;

    start_time_file.close();

    // Read from file
    ifstream break_time_file(".break_total.txt");

    long break_total = 0;

    if( break_time_file >> break_total){
        
    };

    long break_hours = calculate_hour_from_seconds(break_total);
    long break_minutes = calculate_mins_from_seconds(break_total);


    time_t now = get_current_time();

    // Unix gets the total seconds, the difference will be in seconds. Divide by 60 and we get minutes 
    long seconds = static_cast<int>(difftime(now,start_state)) - break_total;
    long hours = calculate_hour_from_seconds(seconds);
    long minutes = calculate_mins_from_seconds(seconds);

    cout << "+---------------------------------------------------------------------------+ \n";
    cout << "\r| Started: " << put_time(localtime(&start_state), "%H:%M \n");
    cout << "\r| Total time worked (break subtracted): "  << hours << " hours and "<< minutes << " minutes." << "\n";
    cout << "\r| Break total: " << (break_hours < 10 ? "0" : "") << break_hours << ":" << (break_minutes < 10 ? "0" : "") <<  break_minutes << "\n";
    cout << "+---------------------------------------------------------------------------+\n";
}

void manual_entry(const string &filename){


    tuple<int, int> hhmm = parse_entry();

    // get today's date
    time_t now = time(nullptr);
    tm local_tm = *localtime(&now);

    // overwrite hour/minute/second
    local_tm.tm_hour = get<0>(hhmm);
    local_tm.tm_min = get<1>(hhmm);
    local_tm.tm_sec = 0;

    // convert to epoch seconds
    time_t started_time = mktime(&local_tm);

    ofstream start_file(filename); 

    string message = "The time entered is: " + to_string(get<0>(hhmm)) + ":" + to_string(get<1>(hhmm)) + ". \n";

    if(!confirm(message)){
        cout << "Input not saved";
        return;
    }

    save_to_file(filename, started_time);
}

void manual_day_entry(){

    if(check_day_started()){
        cout << "Warning. Day already logged as started. Proceeding will overwrite.\n";
    }

    manual_entry(".start_state.txt");

}

void manual_end_entry(){
    if(!check_day_started()){
        throw runtime_error("Day not started. Cannot end non-started day.\n");
    }

    manual_entry(".end_state.txt");
    save_to_log();
}

tuple<int, int> read_epoch_secs_convert_to_hhmm(const string &filename){
    ifstream file(filename);

    time_t epoch = 0;

    file >> epoch;

    struct tm *timeinfo = localtime(&epoch); // convert to local time

    // Convert to hour and minute
    int hour = timeinfo->tm_hour;
    int minute = timeinfo->tm_min;

    return make_tuple(hour, minute);
}

tuple<int, int> parse_entry(){
    int hh = 00;
    int mm = 00;
    char colon = ':';
    string hhmm;
    
    cout << "Input with format: HH:MM \n";
    cin >> hhmm;

    stringstream ss(hhmm);
    ss >> hh >> colon >> mm;

/*     if (!(ss >> hh >> colon >> mm) || colon != ':') {
        throw runtime_error("Invalid format, expected HH:MM");
    } */
    if (hh < 0 || hh > 23) {
        throw runtime_error("Hour must be 0–23");
    }
    if (mm < 0 || mm > 59) {
        throw runtime_error("Minute must be 0–59");
    }

    return make_tuple(hh, mm);
}

// Read the epoch time (seconds) from a file.
long read_from_file(const string &filename){
    ifstream file(filename);

    long tot = 0;
    if(file.is_open()){
        file >> tot;
    }
    file.close();
    return tot;

}

void save_to_file(const string &filename, int tot){
    
    ofstream file(filename);
    if(!file){
        throw runtime_error("Could not open file for writing");
    }
    file << tot;
    file.close();
}

void manual_break_entry(){

    tuple <int, int> hhmm = parse_entry();

    long hh = get<0>(hhmm);
    long mm = get<1>(hhmm);


    long secs = calculate_secs_from_hour_min(hh,mm);

    long tot = read_from_file(".break_total.txt");

    tot += secs;

    string message = "The time entered is: " + to_string(hh) + ":" + to_string(mm) + ". \n";

    if(!confirm(message)){
        cout << "Break time not saved\n";
        return;
    } 
    
    save_to_file(".break_total.txt", tot);

}

time_t get_current_time(){
    auto now_f = system_clock::now();
    time_t now_c = system_clock::to_time_t(now_f); // Now first

    return now_c;
}

void input_thread(){
    string s;

    while(cin >> s){
        if (s == "q"){
            quit = true;
            break;
        }
    }
}


int break_start(){

    time_t now_c = get_current_time();
/*     ofstream file(".break_start.txt");
    
    // Store it as unix timestamp
    file << now_c;
    file.close(); */

    save_to_file(".break_start.txt", now_c);

    thread t(input_thread); // Create thread that checks for input.

    cout << "Break started. Press q to cancel the break. \n";


    int total = 0;
    while (!quit) {

        time_t elapsed = time(nullptr) - now_c;  // seconds since break started
        long hours = calculate_hour_from_seconds(elapsed);
        int minutes = calculate_mins_from_seconds(elapsed);
        int seconds = elapsed % 60;

        cout << "\rOn break: " << hours << ":" << (minutes < 10 ? "0" : "") << minutes << ":" 
        << (seconds < 10 ? "0" : "") << seconds << "   >" << flush;

        total++;
        this_thread::sleep_for(chrono::milliseconds(1000));

    }
    t.join();

    break_stop();
    return 0;
}

int break_stop(){
    ifstream break_start_file(".break_start.txt");

    time_t start;
    break_start_file >> start;
    break_start_file.close();

    time_t now_c = get_current_time();

    // Unix gets the total seconds, the difference will be in seconds. Divide by 60 and we get minutes 
    long seconds = static_cast<int>(difftime(now_c,start));
    long hours = calculate_hour_from_seconds(seconds);
    long minutes = calculate_mins_from_seconds(seconds);

    // Add to the total
    int total = 0;
    ifstream totalFile(".break_total.txt");
    if(totalFile) {
        // If we already have a file, assume it has break already logged, read to total
        totalFile >> total;
    }
    totalFile.close();

    total += seconds;
    int remains = seconds % 60;

    

    cout << "Break ended. Summary: " << hours << setw(2) << setfill('0') << " hours, " << minutes << setw(2) << setfill('0') << " minutes and " << remains << " seconds. \nSave break period? (yes/no) \n";

    string command;
    cin >> command;


    if(command == "no"){
        cout << "Break period not saved. \n";
        return 0;
    }

    cout << "Break period saved. \n";

    ofstream saveTotal(".break_total.txt");
    saveTotal << total;
    saveTotal.close();

    return 0;

}


int start_calculator(){

    if(check_day_started()){
        cerr << "\rDay already started! \n";
        return 0;
    }

    time_t now_c = get_current_time();

    ofstream start_file(".start_state.txt"); 

    /*
    Write the file as: 
    state (unix time)
    time (hour:minutes, ex: 00:40)
    */
    start_file << now_c << "\n";
    //start_file << put_time(localtime(&now_c), "%H:%M \n");

    start_file.close();

    cout << "Day started! Time recorded: " << put_time(localtime(&now_c), "%H:%M \n");

    return 0;
}

void end_calculator(){
    if(!check_day_started()){
        throw runtime_error("Day not started, cannot end time \n"); 
        return;
    }
    
    auto now = system_clock::now();
    time_t end_state = system_clock::to_time_t(now);

    save_to_file(".end_state.txt", end_state);

    save_to_log();

}

void save_to_log(){
    // Extract the break first:

    long break_total = read_from_file(".break_total.txt");

    long break_total_hour = calculate_hour_from_seconds(break_total);
    long break_total_mins = calculate_mins_from_seconds(break_total);

    tuple<int, int> break_hhmm = read_epoch_secs_convert_to_hhmm(".break_total.txt");


    long end_state = read_from_file(".end_state.txt");
    
    long start_state = read_from_file(".start_state.txt");
    
    tuple<int, int> hhmm = read_epoch_secs_convert_to_hhmm(".start_state.txt");

    // Work time (hours:mins)
    long elapsed = end_state - start_state;
    if(elapsed < 0){
        elapsed = 0;
    }
    
    // Total worked time (work time - break)
    long total_work_time = elapsed - break_total;

    long worked_hours = calculate_hour_from_seconds(elapsed);
    long worked_mins = calculate_mins_from_seconds(elapsed);

    long total_work_time_hours = calculate_hour_from_seconds(total_work_time);
    long total_work_time_mins = calculate_mins_from_seconds(total_work_time);

    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

/* 
    ///////////////// UNDER CONSTRUCTION ////////////////////
    string note = "";
    cout << "(Optional): Add note. (c) to ignore. \n";
    getline(std::cin, note);
    ///////////////////////////////////////////////////

     */
    ofstream log_file(DATA_FILE, ios::app); // append mode

    string logging_record = format_record(start_state, end_state, break_total_hour, break_total_mins, worked_hours, worked_mins, total_work_time_hours, total_work_time_mins);


    string message =  "NOTE: The following data will be written and stored: " + logging_record + "\n"
         + "Save this record?\n";

    if(confirm(message)) {
        cout << "Record stored. \n";
        log_file << logging_record;
    } else {
        cout << "Record not stored! \n";
    }

    
    clear_temp_files();

    log_file.close();

    return;
}

string format_record(time_t start_state, time_t end_state, long  break_total_hour,
                     long break_total_mins, long worked_hours,
                     long worked_mins, long total_work_time_hours, long total_work_time_mins) 
{
    ostringstream oss;
    oss << put_time(localtime(&end_state), "%Y-%m-%d")          << ","  // Date
        << put_time(localtime(&start_state), "%H:%M")           << ","  // Start
        << put_time(localtime(&end_state), "%H:%M")             << ","  // End
        << break_total_hour << ":" << setw(2) << setfill('0') << break_total_mins          << ","  // Break Total
        << worked_hours << ":" << setw(2) << setfill('0') << worked_mins << "," // Hours worked
        << total_work_time_hours  << ":" << setw(2) << setfill('0') << total_work_time_mins << "," ;       // Total time
        //<< note << "\n";

    return oss.str();
}

void clear_file(const string& filename) {
    ofstream file(filename, ios::trunc); // open in truncate mode
}

void clear_temp_files(){

    string message = "Clear temporary files? Current data will be erased!";

    if(confirm(message)){
        clear_file(".break_start.txt");
        clear_file(".break_total.txt");
        clear_file(".start_state.txt");
        clear_file(".end_state.txt");

        cout << "Temporary files cleared! \n";

        return;
    }
    cout << "Temporary files not cleared! \n";
}

bool confirm(const string& message) {
    cout << message << "(yes/ENTER to accept, no to cancel): \n";

    string input;
    //cin.ignore(numeric_limits<streamsize>::max(), '\n');  
    //getline(cin, input);

    cin >> input;
    // lowercase input
    transform(input.begin(), input.end(), input.begin(), ::tolower);

    if (input == "yes" || input == "y") {
        return true; // accepted
    }
    return false; // anything else = reject
}
