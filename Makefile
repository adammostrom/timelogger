.PHONY: clean, build

# @ = Hides the command from the output
# - = Wont print if there are any errors

clean:
	-@rm -f .break_start.txt .break_total.txt .start_state.txt
	@touch .break_start.txt .break_total.txt .start_state.txt
	@echo "Temporary datafiles cleaned."

build:
	@g++ timelogger.cpp -o timelogger 

log:
	@cat logged_times.csv

commands:
	@cat Makefile

run:
	@g++ timelogger.cpp -o timelogger;
	@./timelogger