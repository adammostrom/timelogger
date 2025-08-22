#include <iostream>
#include <ctime>
#include <chrono>
#include <fstream>
#include <string>

using namespace std;
using namespace std::chrono;

int break_start();
int break_stop();
int date_calculator();
int start_calculator();
int end_calculator();

int main(){


    int command;
    cout << "Following commands available:\n 1.start \n 2.end \n 3.break \n Input a command: \n";
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
        default:
            cout << "Cancelled or no command given.";
            break;
        }

    


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

    ofstream start_file(".start_temp.txt");
    
    // Store it as unix timestamp
    start_file << now_c;
    start_file.close();

    return 0;
}

int end_calculator(){
    auto now = system_clock::now();
    time_t end_time = system_clock::to_time_t(now);

    ifstream start_file(".start_temp.txt");

    long start_time;
    start_file >> start_time;

    long elapsed = end_time - start_time;


}


int date_calculator(){
        // Get the timestamp for the current date and time
    time_t timestamp;
    time(&timestamp);

        // Display the date and time represented by the timestamp
    cout << ctime(&timestamp);
}

