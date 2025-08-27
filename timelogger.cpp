#include "timelogger.h"

//#include "gui.h"
// #include "gui.h"


atomic<bool> quit(false);

string csv_file = "logged_times.csv";
string csv_testfile = "TEST_logged_times.csv";

string active_log_file = csv_file;

#define TESTING   // uncomment this when testing

#ifdef TESTING
string DATA_FILE = "TEST_logged_times.csv";
#else
string DATA_FILE = "logged_times.csv";
#endif



/* int test_gui(int argc, char **argv){
    GUI gui(250, 100, "Timer App");
    gui.button->callback([](Fl_Widget*, void*){
        cout << "Start clicked! " << endl;
    });
    gui.show(argc, argv);
    return Fl::run();
} */

int main(){

    //test_gui(argc, argv);

    // SHow the current time worked on start
    if(check_day_started()){
        get_current_worked();
    }

    int command;
    cout << "\rFollowing commands available:\n 1.Start \n 2.End \n 3.Break \n 4.Manual day entry \n 5.Clear temporary files \n 6.Cancel \n Input a command: ";
    cin >> command;
    
    switch(command) {
        case 1:
            start_calculator();
            break;
        case 2:
/*             if(!day_started){
                cout << "\rNo day start has been initiated, run the 'manual day start command'\n";
                main();
            } */
            end_calculator();
            break;
        case 3:
            break_start();
            break;
        case 4:
            // For cases when you forget to start the day logger.
            manual_day_entry();
            break;
        case 5:
            clear_temp_files();
            break;
        case 6:
            return 0;
        default:
            cout << "\rCancelled or no command given.";
            break;
        }

    return 0;
}

bool check_day_started(){
    ifstream start_state_file(".start_state.txt");

    long temp = 0;

    start_state_file >> temp;
    start_state_file.close();

    if (temp != 0){
        return true;
        
    }
    return false;

}



void get_current_worked(){

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

    int break_hours = break_total / 3600;
    int break_minutes = (break_total % 3600) / 60;

    if (!break_time_file) {
        break_hours = 0;
        break_minutes = 0;
    }

    time_t now = time(nullptr);
    tm local_tm = *localtime(&now);

    // Unix gets the total seconds, the difference will be in seconds. Divide by 60 and we get minutes 
    int seconds = static_cast<int>(difftime(now,start_state)) - break_total;
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;

    cout << "+---------------------------------------------------------------------------+ \n";
    cout << "\r| Started: " << put_time(localtime(&start_state), "%H:%M \n");
    cout << "\r| Total time worked (break subtracted): "  << hours << " hours and "<< minutes << " minutes." << "\n";
    cout << "\r| Break total: " << break_hours << ":" << break_minutes << "\n";
    cout << "+---------------------------------------------------------------------------+\n";
}

void manual_day_entry(){

    cout << "Input the time the day started with format: HH:MM \n";
    string hhmm;
    cin >> hhmm;
    int hh, mm;
    char colon;

    stringstream ss(hhmm);
    ss >> hh >> colon >> mm;

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

    /*
    Write the file as: 
    state (unix time)
    time (hour:minutes, ex: 00:40)
    */
    start_file << started_time << "\n";
    start_file << put_time(localtime(&started_time), "%H:%M \n");

    start_file.close();

    main();

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
        int minutes = elapsed / 60;
        int seconds = elapsed % 60;

        cout << "\rOn break: " << minutes << ":" 
        << (seconds < 10 ? "0" : "") << seconds << flush;
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

    //Calculate duration
/*     auto now_s = system_clock::now();
    time_t now_c = system_clock::to_time_t(now_s); // Now second */
    time_t now_c = get_current_time();

    // Unix gets the total seconds, the difference will be in seconds. Divide by 60 and we get minutes 
    int seconds = static_cast<int>(difftime(now_c,start));
    int minutes = seconds / 60;

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

    cout << "Break summary: " << minutes << " minutes and " << remains << " seconds." << "\nSave break period? (yes/no) \n";

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



    cout << "Break ended. Duration: " << seconds << " seconds.\nWhich is: " << minutes << " minutes and " << remains << " seconds. \n";

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
    
    // Total worked time (work time - break)
    long total_work_time = elapsed - break_total;

    long worked_hours = calculate_hour_from_seconds(elapsed);
    long worked_mins = calculate_mins_from_seconds(elapsed);

    long total_work_time_hours = calculate_hour_from_seconds(total_work_time);
    long total_work_time_mins = calculate_mins_from_seconds(total_work_time);


    ofstream log_file(DATA_FILE, ios::app); // append mode

    string logging_record = format_record(start_state, end_state, break_total_hour, break_total_mins, worked_hours, worked_mins, total_work_time_hours, total_work_time_mins);

    if(!confirm_logging(logging_record)) {
        cout << "Record not stored! \n";
        return 0;
    }
    cout << "Record stored. \n";
    log_file << logging_record;
    
    clear_temp_files();

    log_file.close();

    return 0;


}

bool confirm_logging(const string& record){
    cout << "NOTE: The following data will be written and stored: " << record
         << "Save this record? (yes/no): \n";
    string input;
    cin >> input;

    // For each character in the string, make it lower case.
    for(auto&c : input) c = tolower(c);

    return (input == "yes" || input == "y");
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
        << total_work_time_hours  << ":" << setw(2) << setfill('0') << total_work_time_mins;        // Total time

    return oss.str();
}

long calculate_hour_from_seconds(long seconds){
    return seconds / 3600;
}
long calculate_mins_from_seconds(long seconds){
    return (seconds % 60) / 60;
}

long read_from_file(const string& file){
    ifstream file_to_read(file);
    long data = 0;
    if(file_to_read >> data){
        cout << "Successfully read from file";
    }
    return data;
}

bool clear_file(const string& filename) {
    ofstream file(filename, ios::trunc); // open in truncate mode
}

void clear_temp_files(){

    cout << "Clear temporary files? Current data will be erased! (yes/no) \n";

    string input;
    cin >> input;

    // For each character in the string, make it lower case.
    for(auto&c : input) c = tolower(c);

    if (input == "yes" || input == "y"){    
        clear_file(".break_start.txt");
        clear_file(".break_total.txt");
        clear_file(".start_state.txt");

        cout << "Temporary files cleared! \n";
    }
}


