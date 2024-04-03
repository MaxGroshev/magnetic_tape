# Magnetic tape
- This program emulates work with magnetic tapes with help of binary files

## Required programs:

-  Cmake version  3.21
-  Boost 1.40 COMPONENTS program_options

<h1 align="center">Install and run</a></h1>

# Install
```
> git clone git@github.com:MaxGroshev/magnetic_tape.git
> mkdir build
> cd build
> cmake ..
> make
```

# Important
### Input and output tapes must <span style="color:red">exist</span> in binary file before run of prog. And their <span style="color:red">sizes are equal<span>

# Run
```
> ./tatlin -keep [src tape] -output [dest tape]
```



# Set config
### It is possible to set delays of writing on tape delay, reading from tape delay, max ram size etc. Go to  "./config/tape_config.conf"  and do it (times are in nanoseconds, but it is not hard to fix it)

```c++
max_ram_size=32600
rewind_delay=0
move_forward_delay=0
move_backward_delay=0
read_delay=0
write_delay=0
```

<h1 align="center">Testing</a></h1>

Required programs:

- Python
- Google tests


## How to run unit tests?
```
> ./tests/unit_tests
```
---




## How to run end to end tests?
Run this command from top project directory
```
> python ./tests/end_to_end_tests/e2e_test.py

```
## Test generator
Required programs:
python
```
> python3 ./tests/end_to_end_tests/test_generator.py

```



