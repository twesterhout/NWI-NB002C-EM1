#E&M1 Homework Week 7

##Building the code
Libraries needed to build the code:
- Boost.Filesystem;
- Boost.IOstreams;
- Boost.System;
- GSL (GNU Scientific Library);
- You also need a gnuplot binary to make the plots.
If you don't have Boost installed, run `install_boost.sh`.
If you don't have GSL installed, run `install_gsl.sh`.

Now run the `compile.sh` script which will generate the `Makefile` used for actual compilation.
Provided Makefile should work on lilo4 (or any other linux system). However, if you use a Windows computer... well, just ssh into lilo4 and it'll save you a lot of trouble. You can execute it by simply running the `make` command.

##Running the code
First, you'll need to edit config.txt file, containing configuration of the coil. Only two shapes are supported now: Circle and Coil. It is quite easy to add more shapes if you have some basic understanding of C++. config.txt must have one of the following forms:
- for Circle:
```
SHAPE: CIRCLE
RADIUS: <number>
CURRENT: <number>
WIRE_RADIUS: <number>
X_MIN: <number>
X_MAX: <number>
X_NR_STEPS: <number>
Y_MIN: <number>
Y_MAX: <number>
Y_NR_STEPS: <number>
Z_MIN: <number>
Z_MAX: <number>
Z_NR_STEPS: <number>
MAX_LEN: <number>
```

- for Coil:
```
SHAPE: COIL
RADIUS: <number>
CURRENT: <number>
NR_TURNS: <number>
LENGTH: <number>
WIRE_RADIUS: <number>
X_MIN: <number>
X_MAX: <number>
X_NR_STEPS: <number>
Y_MIN: <number>
Y_MAX: <number>
Y_NR_STEPS: <number>
Z_MIN: <number>
Z_MAX: <number>
Z_NR_STEPS: <number>
MAX_LEN: <number>
```
where `<number>` stands some number.

Now you can run the program with `./main`. It produces two files: 
- `curve.dat` containing the information about the curve (Circle or Coil);
- `field.dat` containing tha values of the B-field at different points.
Names of the files can be changed by editing the `configure.h` file, which controls the compile time options whereas `config.txt` controls the run-time behavior.


##More control over the code
For more control ove the execution of the program you can edit `configure.h` file. **NB:** Do not forget to recompile the code (for example using `make`).


##Questions/suggestions
Mail to kot.tom97 ad gmail dot com
