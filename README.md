# dining_philosphers_problem
A multithread C program that emulates threads sharing memory. 

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

```sh
Usage : ./dining_philosophers n thinking_duration dining_duration cycle

                   n : number of philosophers to emulate
   thinking_duration : expected time (us) for thinking
     dining_duration : expected time (us) for dining
               cycle : count to repeat cycle: THINKING->HUNGER->EATING

Example : ./dining_philosophers 5 210 20 10
```
