.PHONY: all clean run

# @ = Hides the command from the output
# - = Wont print if there are any errors

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Iinclude

SRC      := main.cpp \
			src/cli.cpp \
			src/logger.cpp \
			src/timer.cpp \
			src/utils.cpp 

OBJ := $(SRC:.cpp=.o) # Converts .cpp → .o

TARGET := timerlogger  # Name of final executable

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)