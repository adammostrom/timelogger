#pragma once
#include <string_view>
#include <filesystem>

// Files

namespace Files {
    
    inline const std::filesystem::path BreakStart    = ".break_start.txt";
    inline const std::filesystem::path BreakTotal    = ".break_total.txt";
    inline const std::filesystem::path BreakEnd      = ".break_end.txt";
    inline const std::filesystem::path SessionEnd    = ".session_end.txt";
    inline const std::filesystem::path SessionStart  = ".session_start.txt";
    inline const std::filesystem::path SessionTotal  = ".session_total.txt";

}

// Directories

inline std::filesystem::path DATA_DIRECTORY = "datafiles";
inline std::filesystem::path WORK_HOURS = "datafiles/work_hours.csv";