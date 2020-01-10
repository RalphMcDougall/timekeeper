# timekeeper
## Introduction
A tool for analyzing code execution times. It has been designed to be language and operating system agnostic. It is able to find the execution times of any snippets of code in any language as long as the timekeeper library has been added for that language. 
Languages supported by the library:
* C++11 and above

## How it works
There are two seperate parts to the project:
* Library: A language specific library records some events in the code that is being timed. At the end of program execution, all of the events get put into a CSV file in the "timekeeper" folder in the program's working directory.

* Analyzer: The [analyzer.py](src/analyzer.py) Python script runs in the background. It gets given a list of folders to scan every few seconds. If it finds a CSV file in the "timekeeper" subfolder, it processes it and creates graphs of the executing time of each code snippet. Furthermore, if the code is part of a timekeeper project, it compares trackers of the same name and compares their execution time.

## Usage guide
Clone this repository into the folder containing all of your source files. Include the timekeeper library in your source code. First run your code and then run the analyzer.

The analyzer has a few options that can be passed as flags in the terminal.

![Analyzer command line flags](readme_images/analyzer_usage.png?raw=true "Analyzer command line flags")

After the analyzer has processed the data for a program execution, a window will pop open with graphs and an appropriate png will will be created in the results folder.

![Program execution analysis](readme_images/program_view.png?raw=true "Program execution analysis")

Furthermore, if the program is part of a larger project, a window will appear containing information about all program executions in the project.

![Project analysis](readme_images/project_view.png?raw=true "Project analysis")

## Requirements
* Target OS: any
* Installed software: Python 3 and the [matplotlib module](https://matplotlib.org/users/installing.html)

## Author
**Ralph McDougall** - [GitHub](https://github.com/RalphMcDougall)

