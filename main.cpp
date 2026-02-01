#include "files.hpp"
#include "cli.hpp"
#include "timer.hpp"
#include "logger.hpp"
#include "utils.hpp"
#include <atomic>

std::vector<Command> commands = {
    {"Start", "s", start_calculator},
    {"End", "e", end_calculator},
    {"Break", "b", break_start},
    {"Store to Log", "sl", save_to_log},
    {"Manual start entry", "md", manual_session_entry},
    {"Manual break entry", "mb", manual_break_entry},
    {"Manual end entry", "me", manual_end_entry},
    {"Clear temporary files", "cl", clear_temp_files_wrapper},
    //{"Logged data overview", "o", logged_data_overview},
    {"Create logging file", "nf", create_logging_file},
    //{"Create new datafile", "nd", create_data_file},
    {"Cancel", "c", ([]()
                     { quit_flag() = true; })}};

void clear_screen()
{
    // POSIX
    std::cout << "\033[2J\033[H"; // Clear screen + move cursor to top-left
}

void move_cursor_top() {
    std::cout << "\033[H"; // Move cursor to top-left without clearing
}


int main()
{

    while (!quit_flag())
    {
        // Show the current time worked on start

        //move_cursor_top();

        show_status();

        // Just prints the commands
        print_main_menu(commands);

        // Waits for a command to call
        Command operation = handle_input(commands);

        // Perform the operation
        operation.action();
    }
    return 0;
}
