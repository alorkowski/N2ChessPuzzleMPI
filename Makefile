CXX = g++
CXXMPI = mpicxx 
CXXFLAGS += -std=c++11 -Wall -O2

SERIAL_SRC = 000-serial/*.cpp
MPI_SRC = 001-mpi/*.cpp

all: examples

examples:
	mkdir -p build
	$(CXX) $(CXXFLAGS) -o build/nqueen-serial $(SERIAL_SRC)
	$(CXXMPI) $(CXXFLAGS) -o build/nqueen-mpi $(MPI_SRC)

clean:
	rm -rf build
