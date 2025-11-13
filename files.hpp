#pragma once
#include <string_view>
#include <filesystem>

// Files

namespace Files {
    
    inline constexpr std::string_view BreakStart  = ".break_start.txt";
    inline constexpr std::string_view BreakTotal  = ".break_total.txt";
    inline constexpr std::string_view EndState    = ".end_state.txt";
    inline constexpr std::string_view StartState  = ".start_state.txt";
    inline constexpr std::string_view LoggedTotal = ".logged_total.txt";

}

// Directories

inline std::filesystem::path DATA_DIRECTORY = "datafiles";
inline std::filesystem::path WORK_HOURS = "datafiles/work_hours.csv";