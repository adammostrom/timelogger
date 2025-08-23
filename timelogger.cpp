#include "timelogger.h"

int main(){


    int command;
    cout << "Following commands available:\n 1.start \n 2.end \n 3.break \n Input a command: \n";
    cin >> command;

    date_calculator();
    
    
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
        default:
            cout << "Cancelled or no command given.";
            break;
        }

    

        //ofstream file("logged_times.csv"); // append mode
    return 0;
}


int break_start(){

    auto now_f = system_clock::now();
    time_t now_c = system_clock::to_time_t(now_f); // Now first

    ofstream file(".break_start.txt");
    
    // Store it as unix timestamp
    file << now_c;
    file.close();

    cout << "Break started. Press q to cancel the break. \n";
    while(true){
        string input;
        cin >> input;            // wait for user input
        if (input == "q") {
            break_stop();
            break;
        }
    }
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

    cout << "Break ended. Duration: " << seconds << " seconds.\nWhich is: " << minutes << " minutes \n";

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

    ifstream break_file(".break_total");

    int break_total;

    break_file >> break_total;
    break_file.close();


    ofstream log_file("logged_times.csv", ios::app); // append mode

    log_file << put_time(std::localtime(&end_state), "%Y-%m-%d")   << ","  // Date
             << start_time                                         << ","  // Start
             << put_time(localtime(&end_state), "%H:%M \n")        << ","  // End
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

