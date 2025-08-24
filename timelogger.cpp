#include "timelogger.h"


atomic<bool> quit(false);
atomic<bool> day_started(false);

int main(){


    int command;
    cout << "Following commands available:\n 1.start \n 2.end \n 3.break \n 4.manual day entry \n Input a command: \n";
    cin >> command;
    
    
    switch(command) {
        case 1:
            start_calculator();
            break;
        case 2:
            if(!day_started){
                cout << "No day start has been initiated, run the 'manual day start command'\n";
                main();
            }
            end_calculator();
            break;
        case 3:
            break_start();
            break;
        case 4:
            // For cases when you forget to start the day logger.
            manual_day_entry();
            break;
        default:
            cout << "Cancelled or no command given.";
            break;
        }

    return 0;
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

    day_started = true;

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

/*     auto now_f = system_clock::now();
    time_t now_c = system_clock::to_time_t(now_f); // Now first */
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
    ifstream ifile(".break_start.txt");

    time_t start;
    ifile >> start;
    ifile.close();

    //Calculate duration
    auto now_s = system_clock::now();
    time_t now_c = system_clock::to_time_t(now_s); // Now second

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

    ofstream saveTotal(".break_total.txt");
    saveTotal << total;
    saveTotal.close();

    int remains = seconds % 60;

    cout << "Break ended. Duration: " << seconds << " seconds.\nWhich is: " << minutes << " minutes and " << remains << " seconds. \n";

    return 0;

}


int start_calculator(){

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

    return 0;
}

int end_calculator(){

    // Extract the break first:

    ifstream break_file(".break_total.txt");

    int break_total;

    break_file >> break_total;

        // Step 2: clear the file (truncate to 0 length)
    {
        ofstream out(".break_total.txt", ios::trunc);
        // file is now empty
    }

    break_file.close();

    cout << "Break Total: " << break_total;

    auto now = system_clock::now();
    time_t end_state = system_clock::to_time_t(now);


    ifstream start_file(".start_state.txt");

    long start_time, start_state;
    // Fetch the time stored in the start_temp.txt
    start_file >> start_state;
    start_file >> start_time;

    long elapsed = end_state - start_state;
    long mins = elapsed / 60;
    long hours = mins / 60;


    ofstream log_file("logged_times.csv", ios::app); // append mode

    log_file << put_time(localtime(&end_state), "%Y-%m-%d")   << ","  // Date
             << put_time(localtime(&start_state), "%H:%M") << ","  // Start
             << put_time(localtime(&end_state), "%H:%M")        << ","  // End
             << break_total                                        << ","  // Break Total
             << hours                                              << "\n";// Total hours

    log_file.close();

    return 0;


}


int date_calculator(){
        // Get the timestamp for the current date and time
/*     time_t timestamp;
    time(&timestamp);

        // Display the date and time represented by the timestamp
    cout << ctime(&timestamp);
    
    ofstream log_file("logged_times.csv");





    log_file << "date" << "," << "start" << ",\n";  // empty End column
    log_file.close(); */

    
}

