#!/bin/bash

######### Adjustable parameters #############
maxNumberOfQueens=10    # N number of NQueens
imax=10 # Maximum number of iterations
maxCPU=4   # Number of processes
#u="-u" # Flag to search for unique solutions
#p="-p" # Flag to reconstruct the game boards
######### End Adjustable parameters #########

n=4
serialCPU=0

awk 'BEGIN {printf("%15s %15s %15s %15s \n" , "NumberOfQueens", "NumberOfCPUs", "AverageTime", "StdDeviation")}'

while [ $n -le $maxNumberOfQueens ];do
    cpuNumber=0
    while [ $cpuNumber -le $maxCPU ];do
	timeArray=()
	i=1
	while [ $i -le $imax ];do
	    if [  $cpuNumber -eq $serialCPU ];then
		time="$(./build/nqueen-serial -n $n $u $p | tail -1 | awk '{print $4}')"
		timeArray+=($time)
	    else
		time="$(mpirun -np $cpuNumber ./build/nqueen-mpi -n $n $u $p | tail -1 | awk '{print $4}')"
		timeArray+=($time)
	    fi
    
	    i=`expr $i + 1`
	done

	ave=$(
	    echo "$timeArray" |
		awk '{sum+=$1}END{print sum/'$imax' }'
	   )
	
	std=$(
	    echo "$timeArray" |
		awk '{sum+=$1; sumsq+=$1*$1}END{print sqrt(sumsq/'$imax' - (sum/'$imax')**2)}'
	   )

	if [  $cpuNumber -eq $serialCPU ];then
	    awk 'BEGIN {printf("%15i %15.8f %15.8f %15.8f \n" ,'$n', 1,'$ave','$std')}'
	else
	    awk 'BEGIN {printf("%15i %15.8f %15.8f %15.8f \n" ,'$n','$cpuNumber','$ave','$std')}'
	fi

	cpuNumber=`expr $cpuNumber + 2`
    done
    n=`expr $n + 1`
done

