#include "utils.hpp"

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
    return (min * 60) + (hour * 3600);
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

bool parse_hhmm(std::string_view s, int &hh, int &mm)
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

    hh = std::stoi(std::string{s.substr(0, 2)});
    mm = std::stoi(std::string{s.substr(2, 2)});

    if (hh > 23 || mm > 59)
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

        int hh, mm;
        if (!parse_hhmm(input, hh, mm))
        {
            std::cout << "Invalid format. Expected HHMM. \n";
            continue;
        }
        return convert_hhmm_to_epoch(hh, mm);
    }
}

std::string format_record(const LogEntry& logEntry)
{
    std::ostringstream oss;
    oss << std::put_time(localtime(&logEntry.end), "%Y-%m-%d") << ","
        << std::put_time(localtime(&logEntry.start), "%H:%M") << ","
        << std::put_time(localtime(&logEntry.end), "%H:%M") << ","
        << logEntry.break_tot_h << ":" << std::setw(2) << std::setfill('0') << logEntry.break_tot_m << ","
        << logEntry.worked_h << ":" << std::setw(2) << std::setfill('0') << logEntry.worked_m << ","
        << logEntry.worked_tot_h << ":" << std::setw(2) << std::setfill('0') << logEntry.worked_tot_m << "\n";
    //<< note << "\n";

    return oss.str();
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

int read_int()
{
    int v;
    if (!(std::cin >> v))
    {
        std::cin.clear();                                                   // clear fail state
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // toss bad input
        std::cout << "Invalid option. Try again.\n";
    }
    return v;
}
