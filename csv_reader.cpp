#include "csv_reader.hpp"
#include "files.hpp"

const int AVERAGE_HOUR_DAY = 8;

/// Read the total amount of entries logged in a csv log file.
int calculate_total_entries(const std::filesystem::path logfile){
    std::filesystem::path p = DATA_DIRECTORY / logfile;
    std::ifstream fin(p.string());
    if(!fin.is_open()){
        std::cerr << "Failed to open file: " << p << '\n';
        return 0;
    }

    std::vector<std::string> row;
    std::string line;

    int count = 0;
    bool first_line = true;

    while (std::getline(fin, line)) {
    
        if (first_line) { // skip header
            first_line = false;
            continue;
        }
        if (line.empty()) continue;
        
        row.clear();

        count++;
    }

    fin.close();

    return count;
}

void compute_total(){

    int total_entries = calculate_total_entries("TEST_logged_times.csv");

    // Hardcoded file. Add option to select which file to check.
    std::filesystem::path p = Files::LoggedTotal.data();
    std::ifstream file(p.string());
    if(!file.is_open()){
        std::cerr << "Failed to open file: " << p << '\n';
        return;
    }

    double total = 0;
    
    // Save total amount of seconds to total.
    // use: read_from_file
    file >> total; 

    // Total hours = total_entries * AVERAGE_HOUR_DAY
    
    double total_seconds = (total_entries * AVERAGE_HOUR_DAY) * 3600;

    double difference_seconds = total - total_seconds;

    int sign = (difference_seconds < 0) ? -1 : 1;
    double totalSecondsAbs = std::abs(difference_seconds);

    int hours = static_cast<int>(totalSecondsAbs / 3600);
    int minutes = static_cast<int>((totalSecondsAbs - hours * 3600) / 60);

    if (sign < 0) hours = -hours;

    std::cout << "Hours: " << hours  << " Minutes: " << (minutes < 10 ? "0" : "") << minutes << "\n";

}

/* void print_total_logged_status(){
    std::cout << "Total entries in " << WORK_HOURS << " : " << std::to_string(calculate_total_entries()) << "\n"; 

    std::cout << "Hours logged based on parameter: " << AVERAGE_HOUR_DAY << " : ";
    compute_total();
    std::cout << "\n";
} */