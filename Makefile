CXX = g++
CPP_FLAGS = -std=c++0x
 
 main: main.o
	$(CXX) $(CPP_FLAGS) main.o -o main

main.o: main.cpp sparse_matrix.hpp
	$(CXX) $(CPP_FLAGS) -c main.cpp -o main.o

.PHONY: clean
clean:
	rm -rf *.o main ./html

documentation:
	doxygen 