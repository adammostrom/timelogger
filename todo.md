[] Add /r for the menu printout so it overwrites and wont stack
[X] Add option on "start" to select either "current time" or manually input time.
    - Added, optimise by adding box for printout and remove "current time check" and replace with other functionality
[] IMPORTANT: Do failchecks on all inputs.
    - [] Do checks on all inputs, and handle faults/bad inputs gracefully. 
    - [] Implement good error handling and bad input handling
[X] Add printout for what time is currently logged on input as "start" and "end" and add "Is this correct?" 
    - confirmation done for end time period, but for start it just shows the start period, except when manually entered.
[X] Add function to modify the start / end time
    - Function to manually input start time exists.
[X] Add method to input custom end time (basically add start and end time manually)
    [] Expand this option to also include previous dates
[] Add method to sort the csv file (maybe with script)
[X] Add function to check "current hours worked"
[X] Add check / confirmation for break when returned
[] Make a function/script to save the logged_times.csv file before any operation on it.
[X] URGENT: Fix the method for checking if a day has started
[X] Fix hours and minutes in the csv log file
[X] Check bug for duplicate entries
[/] Fix so that the "current worked" wont print anything unless the file contains some data
    - Only prints when day is started
[X] Calculate the break parameter in the CSV file to still be HH:MM and dicard seconds all together.
[] Start refactoring all cin and \n into getline and endl instead. (Better for text outputs)
[] IMPORTANT: Implement safe writing to CSV file (never write to actual file!)
[X] Add function to manually set / add a break period
[] Generalise the program so it can be run with any timing event such as study session or work! (Find way to load/create log files) -> Maybe add manager that checks "data storage" for existing files, and asks if the user wants to create a new log or use an existing one, and what to name it
[] Fix the note taking part (make function, clean it of commas, make a safety check for input, a cancel command for skipping notetaking).
 [] Add smart tracker to calculate max amount of break or how many more hours to work to fit 8 hours (or however many you wish to work), maybe this can be done with some kind of countdown
[] Add more functionality for the csv files to parse data and get information
[X] Fix the problem with which dataset to use

