#include "timelogger.h"

int calculate_secs_from_hour_min(int hour, int min);
int main(){

/*     if(check_day_started()){
        cout << "Warning. Day already logged as started. Proceeding will overwrite.\n";
    } */

    int hh = 00;
    int mm = 00;
    char colon = ':';
    string hhmm;
    
    cout << "Input the break elapsed with format: HH:MM \n";
    cin >> hhmm;

    stringstream ss(hhmm);
    ss >> hh >> colon >> mm;

    cout << "After input\n";
    cout << hh << mm << "\n";


    if( hh < 00 || hh > 24){
        cout << "Wrong hour format: " << hh << "\n";
        return 0;
    }
    if(mm < 0 || mm > 60){
        cout << "Wrong minute format: " << mm << "\n";
        return 0;
    }


    long secs = calculate_secs_from_hour_min(hh,mm);

    ifstream tot_break_file("test_add_secs.txt");

    long tot = 0;
    tot_break_file >> tot;

    tot += secs;

    tot_break_file.close();


    ofstream saveTotal("test_add_secs.txt"); 

    string message = "The time entered is: " + to_string(hh) + ":" + to_string(mm) + ". \n";

/*     if(!confirm(message)){
        cout << "Break time not saved";
        return 0;
    } */
    
    saveTotal << tot;
    saveTotal.close();
    return 0;
}

// Right now the user has to just input the total amount of break time to manually add
int calculate_secs_from_hour_min(int hour, int min){
    return (min * 60) + (hour * 3600);
}