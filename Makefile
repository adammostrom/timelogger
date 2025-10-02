.PHONY: clean, build, gui, log, commands, run, test, study

# @ = Hides the command from the output
# - = Wont print if there are any errors

clean:
	-@rm -f .break_start.txt .break_total.txt .start_state.txt
	@touch .break_start.txt .break_total.txt .start_state.txt
	@echo "Temporary datafiles cleaned."

build:
	@git checkout log;
	@g++ timelogger.cpp utils.cpp -o timelogger 
	
commands:
	@cat Makefile

run:
#	git checkout log;
#	git pull;
	@g++ timelogger.cpp utils.cpp -o timelogger;
	@./timelogger

gui:
	@g++ gui.cpp -o gui -lfltk
	@./gui

log:
	git fetch;
	git checkout log;
	git checkout dev -- /datafiles;
	git commit -m "automatic log update";
	git push;
	git checkout dev;
