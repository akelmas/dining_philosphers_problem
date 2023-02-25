# dining_philosphers_problem
A multithread C program that emulates threads sharing memory. 

## Dependencies
* A compatible C Compiler (gcc or clang) with POSIX threads support
* meson >= 1.0.0
* ninja >= 1.11.1
* cmake >= 3.25.2

## Build instructions

1. Clone the repo
2. Setup build 

```sh
meson setup build
```

3. Compile

```sh
meson compile -C build
```

4. Goto build dir `cd build`

```
Usage : ./dining_philosophers n thinking_duration dining_duration cycle

                   n : number of philosophers to emulate
   thinking_duration : expected time (us) for thinking
     dining_duration : expected time (us) for dining
               cycle : count to repeat cycle: THINKING->HUNGER->EATING

Example : ./dining_philosophers 100 100 100 1000

Average hunger duration : 0.002731 sec
Std deviation : 0.000526
                                          Frequency
             0____________________________________________________________________14
    0 - 67   |███████████████
   67 - 134  |█████████████████████████
  134 - 201  |████████████████████████████████████████████████████████████
  201 - 268  |██████████
  268 - 334  |█████████████████████████████████████████████████████████████████████
  334 - 401  |█████████████████████████████████████████████████████████████████████
  401 - 468  |█████████████████████████████████████████████
  468 - 535  |████████████████████
  535 - 602  |██████████████████████████████
  602 - 669  |█████████████████████████
  669 - 736  |████████████████████
  736 - 803  |████████████████████
  803 - 869  |████████████████████████████████████████
  869 - 936  |██████████
  936 - 1003 |███████████████
 1003 - 1070 |██████████
 1070 - 1137 |█████
 1137 - 1204 |██████████

```
