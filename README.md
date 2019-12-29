# timekeeper
## Introduction
A tool for analyzing code execution times. It currently only supports C++ timing, but this will be expanded in the future.

## How it works
There are two seperate parts to the project:
* Library: A language specific library records some events in the code that is being timed. At the end of program execution, all of the events get put into a CSV file in a folder in the pwd of the code being executed.

* Analyzer: The [analyzer.py](src/analyzer.py) Python script runs in the background. It gets given a list of folders to scan every few seconds. If it finds a CSV file that was modified after the last time it checked, it analyzes the data in the file and generates some graphs. This figure is then saved in a PNG file.

## How to use it
Copy the [timekeeper] folder into the directory of the code that you want to time. Run the analyzer and give it the path of the directory where the code resides. Import the relevant library in your code and add trackers by the code that you want to time. Run your code as normal. The analyzer should automatically open the image containing the execution timing graphs.

## Requirements
You need Python3 and matplotlib for the analyzer to work.

## Note
If you are timing C++ code, remember to link the timekeeper object during compilation.
