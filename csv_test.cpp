#include "timelogger.h" 



using namespace std;

int read_file();

int main(){



/* 
    ofstream logged_time ("logged_times.csv");

    if (!logged_time) {
        std::cerr << "Could not open file!\n";
        return 1;
    }

    logged_time << "date,start,end,break_tot,hours_tot"; */
    // Create the file
    auto now = system_clock::now();
    time_t now_c = system_clock::to_time_t(now); // Now first

    ofstream start_file(".start_state.txt"); 

    start_file << now_c << "\n";
    start_file << put_time(localtime(&now_c), "%H:%M \n");

    start_file.close();

    return 0;
}

int read_file(){


    ifstream file(".work_state.txt");

    string test;



}

