#!/bin/bash

# All Benchmarks

RUNS=30
ITERS=50

# Trial -- # THREADS

#   1) NT=68
    NT=2
    #./runall.sh -r $RUNS -i $ITERS -nt $NT

#   2) NT=271
    NT=16
    #./runall.sh -r $RUNS -i $ITERS -nt $NT

#   3) NT=272
    NT=32
    ./runall.sh -r $RUNS -i $ITERS -nt $NT

# Trial 2 -- BIND OPTIONS
# RUNS=30, ITERS=32, NT=32
RUNS=30
ITERS=50
NT=32
#       OpenMP***                   PT
#   1)  P=cores B=spread          Bind Master**
    OP="cores"
    OB="close"
    PM=48
    ./runall.sh -r $RUNS -i $ITERS -nt $NT -bomp $OB -pomp $OP -pmpt $PM
#   2)  P=cores B=close           Bind Kids* **
    OP="cores"
    OB="close"
    PB=136
    ./runall.sh -r $RUNS -i $ITERS -nt $NT -bomp $OB -pomp $OP -bpt $PB
#   3)  P=cores B=false           Bind Master & Kids* **
    OP="cores"
    OB="close"
    PB=24
    PM=268
    # ./runall.sh -r $RUNS -i $ITERS -nt $NT -bomp $OB -pomp $OP -bpt $PB -pmpt $PM

# * kids go to same place for now
# ** chosen PT cores are arbitrary

# Trial 3 -- # ITERS

# NT=68
RUNS=30
NT=32 #

#   1) ITERS=8
    ITERS=75
   # ./runall.sh -r $RUNS -i $ITERS -nt $NT

#   2) ITERS=272 ****
    ITERS=150
   # ./runall.sh -r $RUNS -i $ITERS -nt $NT

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