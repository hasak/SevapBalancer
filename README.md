# SevapBalancer

**SevapBalancer v1.0** is a C++ console application designed to fairly distribute Quran reading assignments (hatma) among a group of readers based on their reading speed (pages per hour). The program ensures that each reader receives a balanced number of pages, randomly distributing consecutive pages to make it fairer.

The project automatically calculates the number of pages each reader should read per session and outputs the results both on the console and to a `.txt` file. It also provides quick facts about estimated completion times.

## Features

* Reads input data from a `.txt` file with reader names and reading speed.
* Calculates a fair distribution of Quran pages for hatma.
* Randomly assigns consecutive pages up to 50 to ensure fairness.
* Displays results in a well-formatted table.
* Outputs results to a `.txt` file for future use.
* Provides quick facts like estimated completion time in years, months, days, hours, minutes, and seconds.
* Includes instructions and developer info.

## Input File Format

Input file (`tempo.txt`) should be structured as follows:

```
Some text or hints

Anything above this delimeter will be ignored
                   |
                   v
--------------------------------
Name1 Surname1 5
Name2 Surname2 3
Name3 Surname3 4
...
```

* Everything above delimeter (`--------------------------------`) is ignored.
* The third column is the number of pages per hour the reader can read.

## How to Run

1. Prepare the input file (`tempo.txt`) in the correct format.
2. Compile the C++ code using a compiler (e.g., Visual Studio or g++).

   ```bash
   g++ -o SevapBalancer SevapBalancer.cpp
   ```
3. Run the program:

   ```bash
   SevapBalancer.exe
   ```
4. Follow the menu options:

   * 1: Instructions
   * 2: Load input file
   * 3: View input data
   * 4: Perform calculation
   * 5: View results
   * 6: Save results to file
   * 7: About the developer
   * 9: Exit

## Output

* Results are displayed on the console in a formatted table showing:

  * Reader number
  * Name and surname
  * Total pages assigned
  * Page ranges for each reader

* Optionally, results can be saved to `out.txt` including:

  * Distribution table
  * Quick facts about completion times
  * Notes for reusing the output file as a new input file

## Notes

* Total pages: `TOTSTR = 604`
* Minimum consecutive pages: `MINSTR = 10`
* Maximum consecutive pages: `MAXSTR = 50`
* Randomization ensures fairness among readers with different speeds.
* Specifically designed for distributing Quran hatma among readers with varying reading speeds.


