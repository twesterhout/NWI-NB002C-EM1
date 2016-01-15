#/bin/bash

echo "Have you used 'install_boost.sh' to install Boost or you had it installed already? (yes/no)"
read used_boost
while [[ $used_boost != "no" && $used_boost != "yes" ]]; do
	echo "please, answer 'yes' or 'no'"
	read used_boost
done

echo "Have you used 'install_gsl.sh' to install GSL or you had it installed already? (yes/no)"
read used_gsl
while [[ $used_boost != "no" && $used_boost != "yes" ]]; do
	echo "please, answer 'yes' or 'no'"
	read used_gsl
done

rm Makefile
if [[ $used_boost == "yes" ]]; then
	BOOST_LIBS="$PWD/boost/lib/libboost_filesystem.a $PWD/boost/lib/libboost_system.a $PWD/boost/lib/libboost_iostreams.a"
	BOOST_INCLUDE="-I$PWD/boost/include"
else
	BOOST_LIBS="-lboost_filesystem -lboost_system -lboost_iostreams"
fi

if [[ $used_gsl == "yes" ]]; then
	GSL_LIBS="$PWD/gsl/lib/libgsl.a $PWD/gsl/lib/libgslcblas.a"
	GSL_INCLUDE="-I$PWD/gsl/include"
else
	GSL_LIBS="-lgsl -lgslcblas"
fi

echo "main: main.cpp configure.h vector3D.h
	g++ -std=c++11 -o main main.cpp -lm $BOOST_LIBS $GSL_LIBS $BOOST_INCLUDE $GSL_INCLUDE" >> Makefile
