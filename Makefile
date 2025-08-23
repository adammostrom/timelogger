.PHONY: clean, build


build:
	g++ timelogger.cpp -o timelogger 

log:
	cat logged_times.csv

commands:
	cat Makefile



# TEMPORARY

run:
	g++ timelogger.cpp -o timelogger ; ./timelogger