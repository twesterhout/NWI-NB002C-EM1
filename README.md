#E&M1 Homework Week 7

##Building the code
Libraries needed to build the code:
- Boost.Filesystem;
- Boost.IOstreams;
- Boost.System;
- GSL (GNU Scientific Library);
- You also need a gnuplot binary to make the plots.

Provided Makefile should work on lilo4 (or any other linux system). However, if you use an OS X or Windows computer... well, just ssh into lilo4 and it'll save you a lot of trouble.


##Running the code
First, you'll need to edit config.txt file, containing configuration of the coil. Only two shapes are supported now: Circle and Coil. It is quite easy to add more shapes if you have some basic understanding of C++. config.txt must have the following form:
- for Circle:
	SHAPE: CIRCLE
	RADIUS: <number>
	CURRENT: <number>
	X_MIN: <number>
	X_MAX: <number>
	X_STEP: <number>
	Y_MIN: <number>
	Y_MAX: <number>
	Y_STEP: <number>
	Z_MIN: <number>
	Z_MAX: <number>
	Z_STEP: <number>
- for Coil:
	SHAPE: COIL
	RADIUS: <number>
	CURRENT: <number>
	NR_TURNS: <number>
	LENGTH: <number>
	X_MIN: <number>
	X_MAX: <number>
	X_STEP: <number>
	Y_MIN: <number>
	Y_MAX: <number>
	Y_STEP: <number>
	Z_MIN: <number>
	Z_MAX: <number>
	Z_STEP: <number>
where <number> stands some number.

Now you can run the program with `./main`. It produces two files: 
- `curve.dat` containing the information about the curve (Circle or Coil);
- `field.dat` containing tha values of the B-field at different points.


##
