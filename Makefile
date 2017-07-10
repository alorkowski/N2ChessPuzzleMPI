CXX = g++
CXXMPI = mpicxx 
CXXFLAGS += -std=c++11 -O2
CXXOMPFLAGS += -std=c++11 -fopenmp -O2

SERIAL_SRC = 000-serial/*.cpp
MPI_SRC = 001-mpi/*.cpp
OMPMPI_SRC = 002-ompmpi/*.cpp
MPI2_SRC = 003-mpi2/*.cpp
OMPMPI2_SRC = 004-ompmpi2/*.cpp
MPIIO_SRC = 005-mpiio/*.cpp

all: serial mpi mpi2

serial:
	mkdir -p build
	$(CXX) $(CXXFLAGS) -o build/nqueen-serial $(SERIAL_SRC)

mpi:
	mkdir -p build
	$(CXXMPI) $(CXXFLAGS) -o build/nqueen-mpi $(MPI_SRC)

ompmpi:
	mkdir -p build
	$(CXXMPI) $(CXXOMPFLAGS) -o build/nqueen-ompmpi $(OMPMPI_SRC)

mpi2:
	mkdir -p build
	$(CXXMPI) $(CXXFLAGS) -o build/nqueen-mpi2 $(MPI2_SRC)

ompmpi2:
	mkdir -p build
	$(CXXMPI) $(CXXOMPFLAGS) -o build/nqueen-ompmpi2 $(OMPMPI2_SRC)

clean:
	rm -rf build
