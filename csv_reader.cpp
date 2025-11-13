#include "csv_reader.hpp"

const int AVERAGE_HOUR_DAY = 8;

/// Read total time from the datafile
int calculate_total_entries(){
    std::filesystem::path p = WORK_HOURS;
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
    if (row.empty()) {
        std::cout << "Last read row is empty (or file had no data)\n";
    }
    std::cout << std::to_string(count)<< "\n";

    return count;
}

void compute_total(){

    int total_entries = calculate_total_entries();

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

    std::cout << hours << ":" << (minutes < 10 ? "0" : "") << minutes << "\n";

}
