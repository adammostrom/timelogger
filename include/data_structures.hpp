#pragma once

#include <string>
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
};
