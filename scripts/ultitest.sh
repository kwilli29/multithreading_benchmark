#!/bin/bash

# All Benchmarks

RUNS=30
ITERS=50

# Trial -- # THREADS

    declare -a threads_array=(2 4 8 16 32 64 128 256 272)

    length1=${#threads_array[@]}
    for (( i=0; i<${length1}; i++ ));
    do
            NT=${threads_array[$i]}
            ./runall.sh -r $RUNS -i $ITERS -nt $NT
    done

# Trial 2 -- BIND OPTIONS
# RUNS=30, ITERS=32, NT=32
RUNS=30
ITERS=50
NT=8

#    OMP: 
#    Bind: false, close, spread [true, master]
#    Places: sockets, cores     [threads]
#
#    PT:
#    Master:            2      8   16  
#    All Children:     4     16  32

# declare -a omp_bind=(false close spread true)
# declare -a omp_places=(sockets cores threads 'x')
# declare -a pthreads_master=(x 2 8 16)
# declare -a pthreads_children=(x 4 16 32)

# length=${#omp_places[@]}
# length2=${#omp_bind[@]}
# for (( i=0; i<${length}; i++ ));
# do
#     for (( j=0; j<${length2}; j++ ));
#     do
#         OP=${omp_places[$i]}
#         OB=${omp_bind[$j]}
#         PM=${pthreads_master[$i]}
#         PB=${pthreads_children[$j]}

#         ./runall.sh -r $RUNS -i $ITERS -nt $NT -bpt $PB -pmpt $PM

#     done
# done

# * kids go to same place for now
# ** chosen PT cores are arbitrary

# Trial 3 -- # ITERS

# RUNS=30
# NT=8 #

#    declare -a iter_array=(2 4 8 16 32 64 128 256 272)
#
#    for numthreads in "${!threads_array[@]}"
#    do
#        NT=$numthreads
#        ./runall.sh -r $RUNS -i $ITERS -nt $NT
#    done


# Pthreads in loops can't have the # of created threads be less than the # of iterations, so Pthreads # iters = 272

# OTHER VARIABLES: mix of other bind options, spawn function, scheduling, grain size, clock, # runs, 

# -r|--runs   -nt|--numthreads)   -i|--iters)   -c|--clock)   -f|--fcn)   
# -bomp|--bindomp)   -pomp|--placesomp)   
# -bpt|--bindpthreads)   -ppt|--placespthreads)   -pmpt|--placemasterpthreads)
    # kids                  n/a                         master
# ***
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