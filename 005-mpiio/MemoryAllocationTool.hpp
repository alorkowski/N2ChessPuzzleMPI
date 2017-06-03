//! MemoryAllocationTool.hpp
/*!
  \brief A class responsible for dynamic allocation of memory.
  \author Lorkowski, Alexander <alexander.lorkowski@epfl.ch>
  \version 1.0
  \date  21 May 2017
  \remark Ecole Polytechnic Federal de Lausanne (EPFL)
  \remark MATH-454 Parallel and High-Performance Computing
*/

#ifndef MEMORYALLOCATIONTOOL_HPP_
#define MEMORYALLOCATIONTOOL_HPP_

class MemoryAllocationTool {
public:

    /*! A method to allocate a 1-dimensional, contiguous integer array.
     *
     * @param nRow The number of rows to allocate.
     * @return A dynamically allocated 1-dimensional, contiguous integer array.
     */
    int* allocate1DInt(int nRow);

    /*! A method to allocate a 2-dimensional, contiguous integer array.
     *
     * @param nRow The number of rows to allocate.
     * @param nColumn The number of columns to allocate.
     * @return A dynamically allocated 1-dimensional, contiguous integer array.
     */
    int** allocate2DInt(int nRow,
                        int nColumn);

    /*! A method to deallocate a 1-dimensional, contiguous integer array.
     *
     * @param array The 1-dimensional, contiguous integer array to deallocate.
     */
    void deallocate1DInt(int* array);

    /*! A method to deallocate a 2-dimensional, contiguous integer array.
     *
     * @param array The 2-dimensional, contiguous integer array to deallocate.
     */
    void deallocate2DInt(int** array);
};

#endif //MEMORYALLOCATIONTOOL_HPP_