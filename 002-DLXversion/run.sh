#!/bin/bash

######### Adjustable parameters #############
n=10    # N number of NQueens
imax=10 # Maximum number of iterations
cpu=4   # Number of processes
#u="-u" # Flag to search for unique solutions
#p="-p" # Flag to reconstruct the game boards
######### End Adjustable parameters #########

i=1
serial_sum=0
#mpi_sum=0

while [ $i -le $imax ];do
    echo "iteration number: " $i
    
    serial_value="$(./nqueen-serial -n $n $u $p | tail -1 | awk '{print $4}')"
    tmp=`echo $serial_sum + $serial_value | bc`    
    serial_sum=$tmp

#    mpi_value="$(mpirun -np $cpu ./nqueen-mpi -n $n $u $p | tail -1 | awk '{print $4}')"
#    tmp=`echo $mpi_sum + $mpi_value | bc`
#    mpi_sum=$tmp
    
    i=`expr $i + 1`
done

awk 'BEGIN {print "serial         : " '$serial_sum'/'$imax' " [s]"; exit}'
#awk 'BEGIN {print "mpi            : " '$mpi_sum'/'$imax' " [s]"; exit}'
