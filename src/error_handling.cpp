#include "error_handling.hpp"


void print_log_error(LogError err){
    switch (err) {
        case LogError::None:
            std::cerr << "No error.\n";
            break;
        case LogError::IoError:
            std::cerr << "Failed I/O operation. \n";
            break;
        case LogError::InvalidName:
            std::cerr << "Invalid filename.\n";
            break;
        case LogError::CreateDirFailed:
            std::cerr << "Failed to create directory.\n";
            break;
        case LogError::CreateFileFailed:
            std::cerr << "Failed to create file.\n";
            break;
        case LogError::SaveToLogFailed:
            std::cerr << "Failed to save to log file.\n";
            break;
        case LogError::SaveToFileFailed:
            std::cerr << "Failed to save to file.\n";
            break;
        case LogError::PermissionDenied:
            std::cerr << "Permission denied.\n";
            break;
        case LogError::ReadFileFailed:
            std::cerr << "Failed to read file.\n";
            break;
        case LogError::FailedToOpenFile:
            std::cerr << "Failed to open file.\n";
            break;
        case LogError::NoneIntegerData:
            std::cerr << "No integer data found.\n";
            break;
        case LogError::OpenFileFailed:
            std::cerr << "Failed to open file.\n";
            break;
        case LogError::CorruptedState:
            std::cerr << "Corrupted state detected.\n";
            break;
        case LogError::FileNotExist:
            std::cerr << "File does not exist.\n";
            break;
        default:
            break;
    }
}