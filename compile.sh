#!/bin/bash

mkdir gnuplot
mkdir gsl
EM1=$PWD

cd gnuplot-5.0.1
./configure --prefix=$EM1/gnuplot
make
make install
cd ..
PATH="$EM1/gnuplot/bin:$PATH:"

#cd gsl-2.1
#./configure --prefix=$EM1/gsl
#make
#make install
#cd ..

make
