#include "../timelogger.h"
#include "../utils.h"
#include <filesystem>
#include <vector>



std::filesystem::path DATA_DIRECTORY = "datafiles";



/// Read total time from the datafile

void calculate_total(){
        // File pointer
    std::fstream fin;

    // Open an existing file
    fin.open("work_hours.csv", std::ios::in);

    // Read the Data from the file
    // as String Vector
    std::vector<std::string> row;
    std::string line, word, temp;

    if(!fin.is_open()){
        std::cout << "Failed to open file";
    }

    while (fin >> temp)
    {
        
        row.clear();
        
        // read an entire row and
        // store it in a string variable 'line'
        getline(fin, line);
        
        std::cout << line;

        // used for breaking words
        std::stringstream s(line);

        // read every column data of a row and
        // store it in a string variable, 'word'
        while (getline(s, word, ','))
        {
            // add all the column data
            // of a row to a vector
            row.push_back(word);
            std::cout << word;
        }
    }

    fin.close();
    for(std::string s : row){
        std::cout << s;
    }
    if(row.empty()){
        std::cout << "List Empty";
    }
}

int main(){
    std::cout << "Hello";
    calculate_total();
}
