#include "Master.hpp"

Master::Master(int n,
               int psize,
               bool unique_flag,
               bool print_flag,
               bool game_flag,
               bool write_flag,
               bool writeGB_flag) :
        numberOfQueens(n),
        numberOfProcessors(psize - 1),
        uniqueFlag(unique_flag),
        printFlag(print_flag),
        gameFlag(game_flag),
        writeFlag(write_flag),
        writeGBFlag(writeGB_flag) {

    offsets[0] = offsetof(mpiData, task);
    offsets[1] = offsetof(mpiData, i);
    offsets[2] = offsetof(mpiData, j);
    offsets[3] = offsetof(mpiData, numberOfSolutions);

    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_taskdetails);
    MPI_Type_commit(&mpi_taskdetails);

    solveAllSolutions();
    if (uniqueFlag) {
        shareAllSolutions();
        solveUniqueSolutions();
    }
    if (printFlag) { printSolutions(); }
    if (gameFlag) { printGameBoard(); }
    if (writeFlag) { writeSolutions(); }
    if (writeGBFlag) { writeGameBoard(); }

    //Don't continue until all workers are idle
    for (proc = 1; proc <= numberOfProcessors; proc++){
        MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    }

    for (proc = 1; proc <= numberOfProcessors; proc++) {
        MPI_Send(&WORK_COMPLETE, 1, MPI_INT, proc, 0, MPI_COMM_WORLD);
    }

    printf("Number of solutions = %i \n",(numberOfSolutions));

    if (uniqueFlag) {
        printf("Number of unique solutions = %i \n",(numberOfUniqueSolutions));
    }
}

Master::~Master() {}

int** Master::alloc_2d_int(int rows, int cols) {
    int *data = (int *)malloc(rows*cols*sizeof(int));
    int **array= (int **)malloc(rows*sizeof(int*));
    for (int i=0; i<rows; i++)
        array[i] = &(data[cols*i]);

    return array;
}

bool Master::subcheck(int i, int j) {
    if (j == i || abs(j - i) == 1){ return true; }
    else { return false; }
}

void Master::solveAllSolutions() {
    taskdetails.task = WORK_COUNT;
    for (int i = 0; i < numberOfQueens; i++) {
        for (int j = 0; j < numberOfQueens; j++) {
            if (!subcheck(i, j)){

                MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
                workerid = status.MPI_SOURCE;

                if (msg == WORK_REQUEST){
                    taskdetails.i = i;
                    taskdetails.j = j;
                    MPI_Send(&taskdetails, 1, mpi_taskdetails, workerid, 0, MPI_COMM_WORLD);
                }
            }else{
                continue;
            }
        }
    }

    //Don't continue until all workers are idle
    for (proc = 1; proc <= numberOfProcessors; proc++){
        MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    }

    //send standby message to workers
    for (proc = 1; proc <= numberOfProcessors; proc++){
        MPI_Send(&WORK_STANDBY, 1, MPI_INT, proc, 0, MPI_COMM_WORLD);
        MPI_Recv(&count, 1, MPI_INT, proc, 0, MPI_COMM_WORLD, &status);

        int **partialSolution;
        partialSolution = alloc_2d_int(count, numberOfQueens);

        MPI_Recv(&(partialSolution[0][0]), count*numberOfQueens, MPI_INT, proc, 0, MPI_COMM_WORLD, &status);

        numberOfSolutions += count;

        for(int i = 0; i < count; i++) {
            Chessboard reconstructedChessboard(numberOfQueens);
            for(int j = 0; j < numberOfQueens; j++) {
                reconstructedChessboard.setState(j, partialSolution[i][j]);
            }
            allSolution.push_back(reconstructedChessboard);
        }

        free(partialSolution[0]);
        free(partialSolution);
    }
}


void Master::shareAllSolutions() {
    for (proc = 1; proc <= numberOfProcessors; proc++){
	taskdetails.task = WORK_SHARE;

	MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
	workerid = status.MPI_SOURCE;

	//double chunk = numberOfSolutions / numberOfProcessors;
	//int start = ( proc - 1 ) * chunk;
	//int end = ( proc * chunk ) - 2;
	
	if (msg == WORK_REQUEST){
	    //int subNumberOfSolutions = ( end - start ) + 1;
	    taskdetails.i = 0;
	    taskdetails.j = 0;
	    taskdetails.numberOfSolutions = numberOfSolutions;
	    MPI_Send(&taskdetails, 1, mpi_taskdetails, workerid, 0, MPI_COMM_WORLD);
	}

	int** contiguousMemoryArray;
	contiguousMemoryArray = alloc_2d_int(numberOfSolutions, numberOfQueens);
	
	for (int i = 0; i < numberOfSolutions; ++i) {
	    for (int j = 0; j < numberOfQueens; ++j) {
		contiguousMemoryArray[i][j] = allSolution.at(i).getState().at(j);
	    }
	}

	MPI_Send(&(contiguousMemoryArray[0][0]),
		 numberOfSolutions*numberOfQueens,
		 MPI_INT,
		 workerid,
		 0,
		 MPI_COMM_WORLD);

	free(contiguousMemoryArray);
    }
}


void Master::solveUniqueSolutions() {
    taskdetails.task = WORK_UNIQUE;
    for (int i = 0; i < numberOfSolutions; i++) {
        MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        workerid = status.MPI_SOURCE;

        if (msg == WORK_REQUEST){
            taskdetails.i = i;
            taskdetails.j = i; //+ (numberOfSolutions/numberOfProcessors);
            MPI_Send(&taskdetails, 1, mpi_taskdetails, workerid, 0, MPI_COMM_WORLD);
        }
    }

    //Don't continue until all workers are idle
    for (proc = 1; proc <= numberOfProcessors; proc++){
        MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    }

    //send standby message to workers
    for (proc = 1; proc <= numberOfProcessors; proc++){
        MPI_Send(&WORK_STANDBY, 1, MPI_INT, proc, 0, MPI_COMM_WORLD);
        MPI_Recv(&count, 1, MPI_INT, proc, 0, MPI_COMM_WORLD, &status);

        int **partialSolution;
        partialSolution = alloc_2d_int(count, numberOfQueens);

	//std::cout << count << std::endl;

        MPI_Recv(&(partialSolution[0][0]), count*numberOfQueens, MPI_INT, proc, 0, MPI_COMM_WORLD, &status);

        numberOfUniqueSolutions += count;

        for(int i = 0; i < count; i++) {
            Chessboard reconstructedChessboard(numberOfQueens);
            for(int j = 0; j < numberOfQueens; j++) {
                reconstructedChessboard.setState(j, partialSolution[i][j]);
            }
            uniqueSolution.push_back(reconstructedChessboard);
        }

	count = 0;

        free(partialSolution[0]);
        free(partialSolution);
    }
}


void Master::printSolutions() {
    std::cout << std::endl << "Printing solutions as arrays" << std::endl;
    for (int i = 0; i < numberOfSolutions; i++) {
        allSolution.at(i).print();
    }
    std::cout << std::endl;
    if (uniqueFlag) {
	std::cout << "Printing unique solutions as arrays" << std::endl;
        for (int i = 0; i < numberOfUniqueSolutions; i++) {
            uniqueSolution.at(i).print();
        }
	std::cout << std::endl;
    }
}


void Master::printGameBoard() {
    for (int i = 0; i < numberOfSolutions; i++) {
        std::cout << std::endl << "No. " << i << std::endl << "**************" << std::endl;
        allSolution.at(i).printGameBoard();
    }
    if (uniqueFlag) {
        for (int i = 0; i < numberOfUniqueSolutions; i++) {
            std::cout << "Unique-No. " << i << std::endl << "**************" << std::endl;
            uniqueSolution.at(i).printGameBoard();
        }
    }
    std::cout << std::endl;
}


void Master::writeSolutions() {
    for (int i = 0; i < numberOfSolutions; i++) {
        // TODO: write solutions here
        if (uniqueFlag) {
            // TODO: write unique solutions here
        }
    }
}


void Master::writeGameBoard() {
    for (int i = 0; i < numberOfSolutions; i++) {
        // TODO: write solutions here in game board format
        if (uniqueFlag) {
            // TODO: write unique solutions here in game board format
        }
    }
}
