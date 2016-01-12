LIBS_BOOST=lib/libboost_filesystem.a lib/libboost_iostreams.a lib/libboost_system.a
LIBS_GSL=-lgsl -lgslcblas -lm

main: main.cpp
	g++ -std=c++11 -o main main.cpp $(LIBS_GSL) $(LIBS_BOOST)
