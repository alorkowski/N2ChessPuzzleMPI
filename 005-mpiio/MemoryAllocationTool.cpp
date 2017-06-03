/*
 * MemoryAllocationTool.cpp
 *
 * DESCRIPTION: Class that handles all dynamic allocation
 *
 * Created on: May 21, 2017
 *
 * AUTHORS:
 * Lorkowski, Alexander <alexander.lorkowski@epfl.ch>
 */

#include "MemoryAllocationTool.hpp"

int* MemoryAllocationTool::allocate1DInt(int nRow) {
    int* array = new int[nRow];
    return array;
}


int** MemoryAllocationTool::allocate2DInt(int nRow,
                                          int nColumn) {
    int* data = new int[nRow*nColumn];
    int** array = new int*[nRow];
    for (int i = 0; i < nRow; ++i, data += nColumn) {
        array[i] = data;
    }

    return array;
}


void MemoryAllocationTool::deallocate1DInt(int* array) {
    delete[] array;
}


void MemoryAllocationTool::deallocate2DInt(int** array) {
    delete[] array[0];
    delete[] array;
}