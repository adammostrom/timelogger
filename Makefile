.PHONY: clean, build, gui, log, commands, run, test, study

# @ = Hides the command from the output
# - = Wont print if there are any errors

clean:
	-@rm -f .break_start.txt .break_total.txt .start_state.txt
	@touch .break_start.txt .break_total.txt .start_state.txt
	@echo "Temporary datafiles cleaned."

build:
	@g++ timelogger.cpp utils.cpp -o timelogger 

log:
	@cat logged_times.csv

commands:
	@cat Makefile

run:
	git checkout log;
	git pull;
	@g++ timelogger.cpp utils.cpp -o timelogger;
	@./timelogger

gui:
	@g++ gui.cpp -o gui -lfltk
	@./gui

test: 
	@g++ timelogger.cpp utils.cpp -DDATA_FILE='"TEST_logged_times.csv"' -o timelogger;
	@./timelogger


study: 
	@g++ timelogger.cpp utils.cpp -DDATA_FILE='"study_hours.csv"' -o timelogger;
	@./timelogger

