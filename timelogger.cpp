#include <iostream>
#include <ctime>
#include <chrono>
#include <fstream>
#include <string>

using namespace std;
using namespace std::chrono;

int break_function(string &command);
int break_stop();

int main(){

    string command;
    cout << "Give a command";
    cin >> command;
    break_function(command);

    return 0;
}
/* 
int chron()
{
    auto start = std::chrono::system_clock::now();
    // Some computation here
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s"
              << std::endl;
} */

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

        cout << "Break ended. Duration: " << seconds << " seconds.\n Which is: " << minutes << " minutes \n";

}


int break_function(string &command){
    if (command == "break"){

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
    if(command == "return"){
        ifstream file(".break_start.txt");
        time_t start;
        file >> start;
        file.close();

        //Calculate duration
        auto now = system_clock::now();
        time_t now_c = system_clock::to_time_t(now);

        // Unix gets the total seconds, the difference will be in seconds. Divide by 60 and we get minutes 
        int minutes = static_cast<int>(difftime(now_c,start) / 60 );

        // Add to the total
        int total = 0;
        ifstream totalFile(".break_total.txt");
        if(totalFile) {
            // If we already have a file, assume it has break already logged, read to total
            totalFile >> total;
        }
        totalFile.close();

        total += minutes;

        ofstream saveTotal(".break_total.txt");
        saveTotal << total;
        saveTotal.close();

        cout << ("Break ended. Duration: %d minutes. \n", minutes);
    }
    return 0;

}

/* 
int date_calculator(){

    // Get the timestamp for the current date and time
    time_t timestamp;
    time(&timestamp);

    

    // Display the date and time represented by the timestamp
    cout << ctime(&timestamp);

    cout << "Choose command: \n";
    int command;
    cin >> command;
    
    if (command == 1){
        cout << "Break started \n";
        auto start = chrono::system_clock::now();

        // Create and open a text file
        ofstream MyFile(".temp.txt");

        // Write to the file
        MyFile << &start;

        // Close the file
        MyFile.close();
    } */

/*     }
    if (command == 2){
        cout << "Break stopped \n";
        auto end = chrono::system_clock::now();
        cout << "Total break: %d, "
    }

    std::chrono::duration<double> elapsed_seconds = end-start; */

    //chron();
}