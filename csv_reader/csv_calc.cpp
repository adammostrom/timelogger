// ...existing code...
#include "../timelogger.h"
#include "../utils.h"
#include <filesystem>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::filesystem::path DATA_DIRECTORY = "datafiles";

/// Read total time from the datafile
void calculate_total(){
    std::filesystem::path p = "work_hours.csv";
    std::ifstream fin(p.string());
    if(!fin.is_open()){
        std::cerr << "Failed to open file: " << p << '\n';
        return;
    }

    std::vector<std::string> row;
    std::string line, word;
    bool first_line = true;

    while (std::getline(fin, line)) {
        if (first_line) { // skip header
            first_line = false;
            continue;
        }
        if (line.empty()) continue;

        row.clear();
        std::stringstream s(line);
        while (std::getline(s, word, ',')) {
            row.push_back(word);
        }

        // Example: print parsed columns for the line
        for (size_t i = 0; i < row.size(); ++i) {
            std::cout << "[" << i << "]=" << row[i] << " ";
        }
        std::cout << '\n';
    }

    fin.close();
    if (row.empty()) {
        std::cout << "Last read row is empty (or file had no data)\n";
    }
}

int main(){
    std::cout << "Hello\n";
    calculate_total();
}