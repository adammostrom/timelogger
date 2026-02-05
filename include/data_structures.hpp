#pragma once

#include <string>
#include <vector>


// Command structure for adding and editing commands in the start menu.
struct Command
{
    std::string name;
    std::string command;
    void (*action)();
};


struct LogEntry {
    long start;
    long end;
    long break_tot_h;
    long break_tot_m;
    long worked_h;
    long worked_m;
    long worked_tot_h;
    long worked_tot_m;
    std::string note;
};

struct StatusParams {
    long start_state;
    long hours;
    long minutes;
    long break_hours;
    long break_minutes;
};

//     file << "date,start,end,break_hour:min,work_hour:min,tot_hour:min,notes \n"; 
struct CsvField {
    std::string name;
};

// Implement in create_log_file
const std::vector<CsvField> logFields = {
    {"DATE"},
    {"START"},
    {"END"},
    {"BREAK(H:M)"},
    {"WORK(H:M)"},
    {"TOTAL(H:M)"},
    {"NOTES"}
};

enum class LogError {
    None,
    InvalidName,
    CreateDirFailed,
    CreateFileFailed,
    SaveToLogFailed,
    PermissionDenied,
    ReadFileFailed,
    FailedToOpenFile,
    NoneIntegerData,
    FileNotExist
};


template <typename T>

struct Result {
    T value;
    LogError error;

    bool ok() const { return error == LogError::None; }
};

