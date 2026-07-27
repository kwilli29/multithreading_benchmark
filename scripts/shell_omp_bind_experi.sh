#!/bin/bash

ARCH=galahad
DATA="data/${ARCH}/OMP_BINDTEST.txt"

# Typical values for OMP_PLACES are:
#    socket : threads are bound to a socket  , but can be moved between cores in the socket;
#    core : threads are bound to a core  , but can be moved between hyper-threads in the core;
#    thread : threads are bound to a specific hyper-threads .

# Values for OMP_PROC_BIND are implementation-defined, but typically:
#   master : threads are bound to the same place as the master thread;
#   close : subsequent thread numbers are placed close together in the defined places;
#   spread : subsequent thread numbers are maxially spread over places;
#   true : threads are bound to their initial placement;
#   false : threads are not bound to their initial placement;

# location:number:stride

# Examples to run an OpenMP threaded application using one to four hardware threads per core using OpenMP 4 affinity:
#    1 thread per tile
#    OMP_PROC_BIND=spread OMP_PLACES=threads OMP_NUM_THREADS=34

#    1 thread per core
#    OMP_PROC_BIND=spread OMP_PLACES=threads OMP_NUM_THREADS=68

#    2 threads per core
#    OMP_PROC_BIND=spread OMP_PLACES=threads OMP_NUM_THREADS=136

#   3 threads per core
#    OMP_PROC_BIND=spread OMP_PLACES=threads OMP_NUM_THREADS=204

#    4 threads per core
#    OMP_PROC_BIND=close OMP_PLACES=threads


# Examples to undersubscribe the cores using OpenMP 4 affinity:

#    1 thread per tile
#    OMP_PROC_BIND=spread OMP_PLACES=”threads(32)” OMP_NUM_THREADS=4

#    1 thread per core
#    OMP_PROC_BIND=spread OMP_PLACES=”threads(32)” OMP_NUM_THREADS=8

#    2 threads per core
#    OMP_PROC_BIND=close OMP_PLACES=”cores(8)” OMP_NUM_THREADS=16

#    3 threads per core
#    OMP_PROC_BIND=close OMP_PLACES=”cores(8)” OMP_NUM_THREADS=24

#    4 threads per core
#    OMP_PROC_BIND=close OMP_PLACES=threads OMP_NUM_THREADS=32

omp_places() {

    cd ../benchmarks/afterspawn/openmp/clock_gettime/
    touch $DATA
#    OMP_PLACES=socket
#    OMP_PLACES=cores
#    OMP_PLACES=threads

    make "$2" # Number
	echo "$3" # Letter
    CURRPROG="$1$2$3"

    RUNS=5
    NT=64

    for((i=0;i<($RUNS);i++)); 
    do	
        export OMP_PLACES=sockets
        echo ",OMP_PROC_BIND= ${OMP_PROC_BIND}, OMP_PLACES= ${OMP_PLACES}" >> $DATA
        ./$CURRPROG $NT >> $DATA

        export OMP_PLACES=cores
        echo ",OMP_PROC_BIND= ${OMP_PROC_BIND}, OMP_PLACES= ${OMP_PLACES}" >> $DATA
        ./$CURRPROG $NT >> $DATA

        export OMP_PLACES=threads
        echo ",OMP_PROC_BIND= ${OMP_PROC_BIND}, OMP_PLACES= ${OMP_PLACES}" >> $DATA
        ./$CURRPROG $NT >> $DATA

    done

    make clean

    cd ../../../../scripts/
}

# rm $DATA

# OMP_PROC_BIND = true
export OMP_PROC_BIND=true
omp_places 0 1 B
omp_places 0 1 C
omp_places 0 1 D
omp_places 0 1 E
omp_places 0 1 F

omp_places 0 2 C
omp_places 0 2 D

omp_places 0 3 C
omp_places 0 3 D
omp_places 0 3 E
omp_places 0 3 F

omp_places 0 4 C
omp_places 0 4 D
omp_places 0 4 E
omp_places 0 4 F

omp_places 0 6 A

# OMP_PROC_BIND = false
export OMP_PROC_BIND=false
omp_places 0 1 B
omp_places 0 1 C
omp_places 0 1 D
omp_places 0 1 E
omp_places 0 1 F

omp_places 0 2 C
omp_places 0 2 D

omp_places 0 3 C
omp_places 0 3 D
omp_places 0 3 E
omp_places 0 3 F

omp_places 0 4 C
omp_places 0 4 D
omp_places 0 4 E
omp_places 0 4 F

omp_places 0 6 A

# OMP_PROC_BIND = master
export OMP_PROC_BIND=master
omp_places 0 1 B
omp_places 0 1 C
omp_places 0 1 D
omp_places 0 1 E
omp_places 0 1 F

omp_places 0 2 C
omp_places 0 2 D

omp_places 0 3 C
omp_places 0 3 D
omp_places 0 3 E
omp_places 0 3 F

omp_places 0 4 C
omp_places 0 4 D
omp_places 0 4 E
omp_places 0 4 F

omp_places 0 6 A

# OMP_PROC_BIND = close
export OMP_PROC_BIND=close
omp_places 0 1 B
omp_places 0 1 C
omp_places 0 1 D
omp_places 0 1 E
omp_places 0 1 F

omp_places 0 2 C
omp_places 0 2 D

omp_places 0 3 C
omp_places 0 3 D
omp_places 0 3 E
omp_places 0 3 F

omp_places 0 4 C
omp_places 0 4 D
omp_places 0 4 E
omp_places 0 4 F

omp_places 0 6 A

# OMP_PROC_BIND = spread
export OMP_PROC_BIND=spread
omp_places 0 1 B
omp_places 0 1 C
omp_places 0 1 D
omp_places 0 1 E
omp_places 0 1 F

omp_places 0 2 C
omp_places 0 2 D

omp_places 0 3 C
omp_places 0 3 D
omp_places 0 3 E
omp_places 0 3 F

omp_places 0 4 C
omp_places 0 4 D
omp_places 0 4 E
omp_places 0 4 F

omp_places 0 6 A

# Bind just master/parent thread to one core
# export OMP_PROC_BIND=true
# export OMP_PLACES="{0:271}"
