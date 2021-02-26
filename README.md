## Project Description
This code solves the dining philosophers problem using semaphores and mutexes. 

This program basically emulates a number of threads sharing same resources.
Each thread represents a philosopher.
the program calcualates average time passed during philosophers thinking
The program accepts 7 arguments

**num_phsp:**    how many thread will be emulated

**min_think:**   the minimum amount of time (sec) that a thread waits before trying to access memory

**max_think:**   the maximum amount  of time (sec) that a thread waits. if a thread reaches maximum wait it immediately tries to access memory

**min_dine:**    the time dining takes at minimum

**max_dine:**    the maximum time until the thread state changes from hungry to thinking

**dst:**         determines the type of distribution to determine waiting amount. 

**num:**         cycle count that how many times threads repeat the process.


## Dependencies/Pre-requisites
No additional tool or library required.
## Setup/Installation

Clone the repostory in a directory
run command in project main directory

`make`

## Usage
After succesfully compiled, execute the command to run the program with default arguments:

`./build/test` 

Also, you can specify arguments like:

`./build/test <num_phsp> <min_think> <max_think> <min_dine> <max_dine> <dst> <num>`

For example:

`./build/test 5 1 2 1 2 "exponential" 3`

After executing this command, output will look like below.

![App screenshot](/data/ss.png)
