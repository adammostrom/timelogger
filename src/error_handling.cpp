#include "error_handling.hpp"


void print_log_error(LogError err){
    switch (err) {
        case LogError::InvalidName:
            std::cerr << "Invalid filename.\n";
            break;
        case LogError::CreateDirFailed:
            std::cerr << "Failed to create directory.\n";
            break;
        case LogError::PermissionDenied:
            std::cerr << "Permission denied.\n";
            break;
        case LogError::CreateFileFailed:
            std::cerr << "Failed to create file.\n";
            break;
        default:
            break;
    }
}