#include "utils.hpp"

// Arithmetic

long calculate_hour_from_seconds(long seconds)
{
    if (seconds <= 0)
    {
        return 0;
    }
    return seconds / 3600;
}
long calculate_mins_from_seconds(long seconds)
{
    if (seconds <= 0)
    {
        return 0;
    }
    return (seconds % 3600) / 60;
}

long calculate_secs_from_hour_min(long hour, long min)
{
    return hour * 3600 + min * 60;
}

long calculate_secs_from_minutes(long minutes){
    if(minutes <= 0){
        return 0;
    }
    return minutes * 60;
}


// Parsing

bool valid_string_hhmm(std::string_view s)
{

    // If input string is not 4 digits, exit
    if (s.size() != 4)
        return false;

    // if they are not all digits, exit
    // for each element in the string, call the lambda function to check if the character is a digit.
    if (!std::all_of(s.begin(), s.end(),
                     [](unsigned char c)
                     { return std::isdigit(c); }))
        return false;
    return true;
}

bool parse_hhmm_helper(int hh, int mm)
{
    if (hh < 0 || mm < 0)
    {
        return false;
    }
    if (hh > 24 || mm > 60)
        return false;
    return true;
}




std::optional<time_t> prompt_hhmm()
{

    while (true)
    {
        std::cout << "Input with format: HHMM (c to cancel)\n";
        std::string input;
        std::cin >> input;

        if (input == "c")
        {
            return std::nullopt;
        }

        if (!valid_string_hhmm(input))
        {
            std::cout << "Invalid format. Expected HHMM. \n";
            continue;
        }

        long hh = std::stoi(std::string{input.substr(0, 2)});
        long mm = std::stoi(std::string{input.substr(2, 2)});

        if (!parse_hhmm_helper(hh, mm))
        {
            std::cout << "HH must be in range 0-24 and MM in range 0-60\n";
            continue;
        }

        return convert_hhmm_to_epoch(hh, mm);
    }
}

// Formatting

std::string format_record(const LogEntry& entry, const std::tm& start_tm, const std::tm& end_tm) {
    std::ostringstream oss;
    oss << std::put_time(&end_tm, "%Y-%m-%d") << ","
        << std::put_time(&start_tm, "%H:%M") << ","
        << std::put_time(&end_tm, "%H:%M") << ","
        << entry.break_tot_h << ":" << std::setw(2) << std::setfill('0') << entry.break_tot_m << ","
        << entry.worked_h << ":" << std::setw(2) << std::setfill('0') << entry.worked_m << ","
        << entry.worked_tot_h << ":" << std::setw(2) << std::setfill('0') << entry.worked_tot_m << ","
        << entry.note << "\n";
    return oss.str();
}
// Directory

/* Check to see if the directory with the datafiles exists. If none exist for some reason, it will be created. */
// Pure logic: returns true if directory exists or was successfully created
bool ensure_directory_exists(const std::string &directory)
{
    if (std::filesystem::exists(directory))
        return true;

    try
    {
        return std::filesystem::create_directory(directory);
    }
    catch (const std::filesystem::filesystem_error &)
    {
        return false;
    }
}

std::vector<std::string> read_from_directory(const std::string &path)
{
    std::vector<std::string> files;
    if (path.empty())
        return files;
    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        files.push_back(entry.path().filename());
    }
    return files;
}