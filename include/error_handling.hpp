#pragma once
//#include "data_structures.hpp"

#include <iostream>
#include <string>

enum class LogError
{
    None,
    InvalidName,
    IoError,
    CreateDirFailed,
    CreateFileFailed,
    SaveToLogFailed,
    SaveToFileFailed,
    PermissionDenied,
    ReadFileFailed,
    FailedToOpenFile,
    NoneIntegerData,
    OpenFileFailed,
    CorruptedState,
    FileNotExist
};

template <typename T>

struct Result
{
    T value;
    LogError error;

    bool ok() const { return error == LogError::None; }
};

void print_log_error(LogError err);