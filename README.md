# Multiple threads sharing memory (Dining philosophers)
This code solves the dining philosophers problem using semaphores and mutexes. 

This program basically emulates a number of threads sharing same resources.
Each thread represents a philosopher.
the program calcualates average time passed during philosophers thinking
The program accepts 7 arguments

**num_phsp:**    _how many thread will be emulated_

**min_think:**   _the minimum amount of time (sec) that a thread waits before trying to access memory_

**max_think:**   _the maximum amount  of time (sec) that a thread waits. if a thread reaches maximum wait it immediately tries to access memory_

**min_dine:**    _the time dining takes at minimum_

**max_dine:**    _the maximum time until the thread state changes from hungry to thinking_

**dst:**         _determines the type of distribution to determine waiting amount._

**num:**         _cycle count that how many times threads repeat the process._


## Dependencies/Pre-requisites
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/) with POSIX threads

## Setup/Installation
1. Clone this repo. 
2. Run the command: `make`
3. Run it: `./build/test`.

## Usage
After succesfully compiled, execute the command to run the program with default arguments:

`./build/test` 

Also, you can specify arguments like:

`./build/test <num_phsp> <min_think> <max_think> <min_dine> <max_dine> <dst> <num>`

For example:

`./build/test 5 1 2 1 2 "exponential" 3`

After executing this command, output will look like below.

![App screenshot](/data/ss.png)
