[] Add /r for the menu printout so it overwrites and wont stack
[/] Add option on "start" to select either "current time" or manually input time.
    - Added, optimise by adding box for printout and remove "current time check" and replace with other functionality
[] Do failchecks on all inputs
[/] Add printout for what time is currently logged on input as "start" and "end" and add "Is this correct?" 
    - confirmation done for end time period, but for start it just shows the start period, except when manually entered.
[/] Add function to modify the start / end time
    - Function to manually input start time exists.
[] Add method to input custom end time (basically add start and end time manually)
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
[] Calculate the break parameter in the CSV file to still be HH:MM and dicard second all together.
[] Start refactoring all cin and \n into getline and endl instead. (Better for text outputs)
[] IMPORTANT: Implement safe writing to CSV file (never write to actual file!)
[X] Add function to manually set / add a break period