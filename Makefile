.PHONY: all clean run

# @ = Hides the command from the output
# - = Wont print if there are any errors

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Iinclude
CXXTESTFLAGS := $(CXXFLAGS) -fsanitize=undefined 

SRC       := \
	src/cli.cpp \
	src/logger.cpp \
	src/timer.cpp \
	src/utils.cpp \
	src/error_handling.cpp

TMP        := \
		.break_start.txt \
		.break_total.txt \
		.session_end.txt \
		.session_start.txt

TEST_SRC := tests/test_utils.cpp
MAIN_SRC  := main.cpp


SRC_OBJ := $(SRC:.cpp=.o) # Converts .cpp → .o
MAIN_OBJ  := $(MAIN_SRC:.cpp=.o)
TEST_OBJ := $(TEST_SRC:.cpp=.o)


TARGET := timerlogger  # Name of final executable
TEST_TARGET  := test_logger

$(TARGET): $(SRC_OBJ) $(MAIN_OBJ)
	$(CXX) $^ -o $@


all: $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(SRC_OBJ) $(MAIN_OBJ) $(TEST_OBJ) $(TARGET) $(TEST_TARGET) $(TMP)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(SRC_OBJ) $(TEST_OBJ)
	$(CXX) $(CXXTESTFLAGS) $^ -o $@
