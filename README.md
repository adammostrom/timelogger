# TimerLogger

A minimal CLI time-tracking tool written in C++17.

I built this to track my work and study hours without relying on third-party software.
It logs sessions to user-created CSV files, allowing separate logs for different projects or study blocks.

The project is still evolving, but core functionality works.

## Features

- Start and end work/study sessions
- Track break duration
- Append session data to CSV files
- Lightweight CLI interface

## Requirements

- g++ (C++17 compatible)
- make

Tested with:
- g++ 11+
- Linux (should work anywhere with a C++17 toolchain)

## Running

Just clone the repo and run 

```
make build; make run
```
