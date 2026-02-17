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

struct LogEntry
{
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

struct StatusParams
{
    long start_state;
    long end_state;
    long total;
    long break_start;
    long break_total;
};

enum class ConfirmResult
{
    Yes,
    No,
    Cancel
};

//     file << "date,start,end,break_hour:min,work_hour:min,tot_hour:min,notes \n";
struct CsvField
{
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
    {"NOTES"}};
