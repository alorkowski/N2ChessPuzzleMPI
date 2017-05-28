CXX = g++
CXXMPI = mpicxx 
CXXFLAGS += -std=c++11 -Wall -O2
CXXOMPFLAGS += -std=c++11 -Wall -O2 -fopenmp

SERIAL_SRC = 000-serial/*.cpp
MPI_SRC = 001-mpi/*.cpp
MPMPI_SRC = 002-mpmpi/*.cpp
FAST_SERIAL_SRC = 003-fastSerial/*.cpp

all: serial mpi mpmpi

serial:
	mkdir -p build
	$(CXX) $(CXXFLAGS) -o build/nqueen-serial $(SERIAL_SRC)

mpi:
	mkdir -p build
	$(CXXMPI) $(CXXFLAGS) -o build/nqueen-mpi $(MPI_SRC)

mpmpi:
	mkdir -p build
	$(CXXMPI) $(CXXOMPFLAGS) -o build/nqueen-mpmpi $(MPMPI_SRC)

clean:
	rm -rf build
