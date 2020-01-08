# timekeeper
## Introduction
A tool for analyzing code execution times. It has been designed to be language and operating system agnostic. It is able to find the execution times of any snippets of code in any language as long as the timekeeper library has been added for that language. The library has currently been created for C++11 and above.

## How it works
There are two seperate parts to the project:
* Library: A language specific library records some events in the code that is being timed. At the end of program execution, all of the events get put into a CSV file in the "timekeeper" folder in the program's working directory.

* Analyzer: The [analyzer.py](src/analyzer.py) Python script runs in the background. It gets given a list of folders to scan every few seconds. If it finds a CSV file in the "timekeeper" subfolder, it processes it and creates graphs of the executing time of each code snippet. Furthermore, if the code is part of a timekeeper project, it compares trackers of the same name and compares their execution time.

## How to use it
Copy the [timekeeper](timekeeper) folder into the directory of the code that you want to time. Run the analyzer and give it the path of the directory where the code resides. Import the relevant library in your code and add trackers by the code that you want to time. Run your code as normal. The analyzer should automatically open the image containing the execution timing graphs.

## Requirements
* Target OS: any
* Installed software: Python 3 and the [matplotlib module](https://matplotlib.org/users/installing.html)

