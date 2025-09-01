#include "timelogger.h"

//#include "gui.h"
// #include "gui.h"


atomic<bool> quit(false);

string csv_file = "logged_times.csv";
string csv_testfile = "TEST_logged_times.csv";

string active_log_file = csv_file;

//#define TESTING   // uncomment this when testing

#ifdef TESTING
string DATA_FILE = "TEST_logged_times.csv";
#else
string DATA_FILE = "logged_times.csv";
#endif

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
    cout << "\rFollowing commands available:\n 1.Start \n 2.End \n 3.Break \n 4.Manual day entry \n 5.Clear temporary files \n 6.Manual break entry \n 7. Cancel \n Input a command: ";
    cin >> command;
    
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
            clear_temp_files();
            break;
        case 6:
            manual_break_entry();
            break;
        case 7:
            return 0;
        
        default:
            cout << "\rCancelled or no command given.";
            break;
        }

    return 0;
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
}

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

    int break_hours = calculate_hour_from_seconds(break_total);
    int break_minutes = calculate_mins_from_seconds(break_total);


    time_t now = get_current_time();

    // Unix gets the total seconds, the difference will be in seconds. Divide by 60 and we get minutes 
    int seconds = static_cast<int>(difftime(now,start_state)) - break_total;
    int hours = calculate_hour_from_seconds(seconds);
    int minutes = calculate_mins_from_seconds(seconds);

    cout << "+---------------------------------------------------------------------------+ \n";
    cout << "\r| Started: " << put_time(localtime(&start_state), "%H:%M \n");
    cout << "\r| Total time worked (break subtracted): "  << hours << " hours and "<< minutes << " minutes." << "\n";
    cout << "\r| Break total: " << break_hours << ":" << break_minutes << "\n";
    cout << "+---------------------------------------------------------------------------+\n";
}

void manual_day_entry(){

    if(check_day_started()){
        cout << "Warning. Day already logged as started. Proceeding will overwrite.\n";
    }

    int hh = 00;
    int mm = 00;
    char colon = ':';
    string hhmm;
    
    cout << "Input the time the day started with format: HH:MM \n";
    cin >> hhmm;

    stringstream ss(hhmm);
    ss >> hh >> colon >> mm;

    if( hh < 00 || hh > 24){
        cout << "Wrong hour format: " << hh << "\n";
        return;
    }
    if(mm < 0 || mm > 60){
        cout << "Wrong minute format: " << mm << "\n";
        return;
    }

    cout << "SHOULD NOT PRINT";
    // get today's date
    time_t now = time(nullptr);
    tm local_tm = *localtime(&now);

    // overwrite hour/minute/second
    local_tm.tm_hour = hh;
    local_tm.tm_min = mm;
    local_tm.tm_sec = 0;

    // convert to epoch seconds
    time_t started_time = mktime(&local_tm);

    ofstream start_file(".start_state.txt"); 

    string message = "The time entered is: " + to_string(hh) + ":" + to_string(mm) + ". \n";

    if(!confirm(message)){
        cout << "Start time not saved";
        return;
    }
    
    start_file << started_time << "\n";
    start_file << put_time(localtime(&started_time), "%H:%M \n");
    start_file.close();

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
    ofstream file(".break_start.txt");
    
    // Store it as unix timestamp
    file << now_c;
    file.close();

    thread t(input_thread); // Create thread that checks for input.

    cout << "Break started. Press q to cancel the break. \n";


    int total = 0;
    while (!quit) {

        time_t elapsed = time(nullptr) - now_c;  // seconds since break started
        long hours = elapsed / 3600;
        int minutes = elapsed / 60;
        int seconds = elapsed % 60;

        cout << "\rOn break: " << hours << ":" << (minutes < 10 ? "0" : "") << minutes << ":" 
        << (seconds < 10 ? "0" : "") << seconds << "\n"<< flush;

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

    // Create the file
    auto now = system_clock::now();
    time_t now_c = system_clock::to_time_t(now); // Now first

    ofstream start_file(".start_state.txt"); 

    /*
    Write the file as: 
    state (unix time)
    time (hour:minutes, ex: 00:40)
    */
    start_file << now_c << "\n";
    start_file << put_time(localtime(&now_c), "%H:%M \n");

    start_file.close();

    cout << "Day started! Time recorded: " << put_time(localtime(&now_c), "%H:%M \n");

    return 0;
}

int end_calculator(){

    // Extract the break first:

    ifstream break_file(".break_total.txt");

    long break_total = 0;

    break_file >> break_total;

        // Step 2: clear the file (truncate to 0 length)
    {
        ofstream out(".break_total.txt", ios::trunc);
        // file is now empty
    }

    break_file.close();

    long break_total_hour = calculate_hour_from_seconds(break_total);
    long break_total_mins = calculate_mins_from_seconds(break_total);


    auto now = system_clock::now();
    time_t end_state = system_clock::to_time_t(now);


    ifstream start_file(".start_state.txt");

    long start_time = 0;
    long start_state = 0;
    // Fetch the time stored in the start_temp.txt
    start_file >> start_state;
    start_file >> start_time;

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

    return 0;


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
        << total_work_time_hours  << ":" << setw(2) << setfill('0') << total_work_time_mins << "\n";        // Total time

    return oss.str();
}

long calculate_hour_from_seconds(long seconds){
    if(seconds <= 0){
        return 0;
    }
    return seconds / 3600;
}
long calculate_mins_from_seconds(long seconds){
    if(seconds <= 0){
        return 0;
    }
    return (seconds % 3600) / 60;
}


bool clear_file(const string& filename) {
    ofstream file(filename, ios::trunc); // open in truncate mode
}

void clear_temp_files(){

    string message = "Clear temporary files? Current data will be erased!";

    if(confirm(message)){
        clear_file(".break_start.txt");
        clear_file(".break_total.txt");
        clear_file(".start_state.txt");

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


// Right now the user has to just input the total amount of break time to manually add
int calculate_secs_from_hour_min(int hour, int min){
    return (min * 60) + (hour * 3600);
}