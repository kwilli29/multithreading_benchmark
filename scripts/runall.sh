#!/bin/bash

ARCH="macbook" # $1
LINUX=1
if [ "$ARCH" = "macbook" ]; then
    LINUX=0
fi

# OPTIONS:

# # RUNS
# # THREADS
# # ITERS
# 
# CLOCK
# BIND
# OTHER --> OMP SCHEDULE, PT DETACHED?, 
# FUNCTION?

## Maybe in-take option to file to process flags

# Command Template:
# ./run-benchmark -r 50 -i 50 -t 32 -f fast -c CLOCK_GETTIME 
#   -h or --help:
#       -r or --run: # of time to run every benchmark
#       -i or --iter: # of iterations in loop benchmarks
#       -t or --thread: # of threads
#       -f or --fcn: which fcn to use: fast, slow, etc.
#       -c or --clock:  which clock to use: clock_gettime, gettimeofday, platform-specific clock

##### Language-Dependent Flags ####
####   GRAIN SIZE   #### --> later?
#       CILK --> cilk cannot have a variable for a gs
#       OMP

####   BIND & PLACES        ####
#   -bomp bind,places
#       OMP
#       BIND = false,true,close,spread,master
#       PLACES = sockets,cores,threads
#   -bpt
#       PT
#       CORE_ID = user puts in a code
#       later: a rudimentary core placement scheme


####   SCHEDULE     ####
#   -s
#       OMP
#       PT
#       CILK

# in cilk --> print 'bind vars not applicable'

#### #### #### #### 

#### #### #### ####

# METRICS: 
#       Mean
#       Median
#       Std. Dev.
#       

# OUTPUT:
#   CSV
#   EXCEL SHEET
# LATER: MORE OUTPUT OPTIONS?

#### #### PROCESS INPUT #### ####

#### #### #### #### #### ####

#### #### PROCESS FLAGS #### ####

RUNS=5 # 50 # 10
NUM_THREADS=50 # maybe could make array options for the user to run alot of tests at once
ITERS=50

CLOCK="clock_gettime"   # clock_gettime, gettimeofday, platform
FCN="fast"              # fast, slow

CILK_GRAINSIZE=1
OMP_GRAINSIZE=1
OMP_BIND=""         # true, false, master, close, spread
OMP_PLACES=""       # sockets, cores, threads

PT_BIND="x"          #
PT_PLACES="x"        #
PT_MASTER="x"        # 

# Things that might need separate scripts: Loop Scheduling, Grain Size, Functionsma
OMP_SCHED=""
PT_SCHED=""
OTHER=""

# Loop through all arguments
while [[ $# -gt 0 ]]; do
  case "$1" in
    -h|--help)
      echo "Usage: $0 [options]"
      exit 0
      ;;
    -r|--runs)
      RUNS="$2"
      shift 2 # Move past the flag
      ;;
    -nt|--numthreads)
      NUM_THREADS="$2"
      shift 2 # Move past the flag
      ;;
    -i|--iters)
      ITERS="$2"
      shift 2 # Move past the flag
      ;;   
    -c|--clock)
      CLOCK="$2"
      shift 2 # Move past the flag
      ;;
    -f|--fcn)
      FCN="$2"
      shift 2 # Move past the flag
      ;;         
    -bomp|--bindomp)
      OMP_BIND="$2"
      shift 2 # Move past the flag
      ;;
    -pomp|--placesomp)
      OMP_PLACES="$2"           # !!!! Change to comma-separated bomp flag
      shift 2 # Move past the flag
      ;;
    -bpt|--bindpthreads)
      PT_BIND="$2"
      shift 2 # Move past the flag
      ;;
    -ppt|--placespthreads)
      PT_PLACES="$2"              # !!!! Change to comma-separated bpt flag
      shift 2 # Move past the flag
      ;;
    -pmpt|--placemasterpthreads)
      PT_MASTER="$2"              # !!!! Change to comma-separated pmpt flag
      shift 2 # Move past the flag
      ;;
    *)
      echo "Unknown option: $1"
      exit 1
      ;;
  esac
done

#### #### #### #### #### ####

cilk_benchmarks() {
    cd cilk/${CLOCK}

    # Bind Settings
    # Schedule Settings
    # Other Settings

    make $1

    # GATHER EXECUTABLES & EXECUTE
    for((i=0;i<($RUNS);i++)); 
	do
        for f in ./*; do
            if [[ -x "$f" && "${FN}" != "Makefile" ]]; then
                FN="$(basename -- "$f")"
                CILK_DATA="data/${FN}_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}.csv"
                if [ ! -d "$f" ]; then
                    if [ ! -f ${CILK_DATA} ]; then
                        touch ${CILK_DATA}
                    fi
                    CILK_NWORKERS=$NUM_THREADS ./${f} $NUM_THREADS $ITERS >> ${CILK_DATA}
                fi
            fi
        done
    done

    LANG="cilk"

    # METRICS
    for f in data/*; do
        if [[ ! -d "$f" ]]; then
            PATTERN="^data/[A-Z0-9]+_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}\.csv$"
            if [[ "$f" =~ $PATTERN ]]; then
                # file , nt , lang , runs , iters , clock , fcn , [bind settings] , [other settings] , arch
                python3 ./../../../../scripts/process_metrics.py ${f} $NUM_THREADS $LANG $RUNS $ITERS $CLOCK $FCN "[]" "[]" $ARCH >> ../../../../scripts/${OUTPUT}
            fi
        fi
    done

    make clean &>/dev/null

    cd ../..
}
openmp_benchmarks() {

    cd openmp/${CLOCK}

    # Bind Settings
    # if omp_bind is not ""
    if [[ $OMP_BIND != "" ]]; then
        echo ",,,bindomp: $OMP_BIND"
        export OMP_PROC_BIND=$OMP_BIND
    fi
    # if omp_place is not ""
    if [[ $OMP_PLACES != "" ]]; then
        echo ",,,bindplaces: $OMP_PLACES"
        export OMP_PROC_PLACES=$OMP_PLACES
    fi
    # Schedule Settings
    # Other Settings

    # COMPILE
    make $1
    
    # GATHER EXECUTABLES & EXECUTE
    for((i=0;i<($RUNS);i++)); 
	do
        for f in ./*; do
            if [[ -x "$f" && "${FN}" != "Makefile" ]]; then
                FN="$(basename -- "$f")"
                OPENMP_DATA="data/${FN}_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_${OMP_BIND}_${OMP_PLACES}.csv"
                if [ ! -d "$f" ]; then
                    if [ ! -f ${OPENMP_DATA} ]; then
                        touch ${OPENMP_DATA}
                    fi
                    ./${f} $NUM_THREADS $ITERS >> ${OPENMP_DATA}
                fi
            fi
        done
    done

    LANG="openmp"

    # METRICS
    for f in data/*; do
        if [[ ! -d "$f" ]]; then
            PATTERN="^data/[A-Z0-9]+_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_${OMP_BIND}_${OMP_PLACES}\.csv$"
            if [[ "$f" =~ $PATTERN ]]; then
                # file , nt , lang , runs , iters , clock , fcn , [bind settings] , [other settings] , arch
                python3 ./../../../../scripts/process_metrics.py ${f} $NUM_THREADS $LANG $RUNS $ITERS $CLOCK $FCN "[${OMP_BIND},${OMP_PLACES}]" "[]" $ARCH >> ../../../../scripts/${OUTPUT}
            fi
        fi
    done

    # CLEAN
    make clean &>/dev/null

    cd ../..

}
pthreads_benchmarks() { # pthread_self()

    cd pthreads/${CLOCK}

    PTNT=$NUM_THREADS

    # Bind Settings
    if [[ $PT_BIND != "x" ]]; then
        echo ",,,bindpt: $PT_BIND"
    fi
    if [[ $PT_PLACES != "x" ]]; then
        echo ",,,placespt: $PT_PLACES"
    fi
    if [[ $PT_MASTER != "x" ]]; then
        echo ",,,placeptmaster: $PT_MASTER"
    fi
    # Schedule Settings
    # Other Settings

    if (( $NUM_THREADS < $ITERS )); then # !!!!
        PTNT=$ITERS
    fi

    # COMPILE
    make $1

    # GATHER EXECUTABLES & EXECUTE
    for((i=0;i<($RUNS);i++)); 
	do
        for f in ./*; do
            if [[ -x "$f" && "${FN}" != "Makefile" ]]; then
                FN="$(basename -- "$f")"
                PTHREADS_DATA="data/${FN}_${PTNT}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_${PT_BIND}_${PT_PLACES}_${PT_MASTER}.csv"
                if [ ! -d "$f" ]; then
                    if [ ! -f ${PTHREADS_DATA} ]; then
                        touch ${PTHREADS_DATA}
                    fi
                    ./${f} $PTNT $ITERS $PT_BIND $PT_PLACES $PT_MASTER >> ${PTHREADS_DATA}
                fi
            fi
        done
    done

    LANG="pthreads"

    # METRICS
    for f in data/*; do
        if [[ ! -d "$f" ]]; then
            PATTERN="^data/[A-Z0-9]+_${PTNT}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_${PT_BIND}_${PT_PLACES}_${PT_MASTER}\.csv$"
            if [[ "$f" =~ $PATTERN ]]; then
                # file , nt , lang , runs , iters , clock , fcn , [bind settings] , [other settings] , arch
                python3 ./../../../../scripts/process_metrics.py ${f} $PTNT $LANG $RUNS $ITERS $CLOCK $FCN "[${PT_BIND},${PT_PLACES},${PT_MASTER}]" "[]" $ARCH >> ../../../../scripts/${OUTPUT}
            fi
        fi
    done


    # CLEAN
    make clean &>/dev/null

    cd ../..
}
pthreads_mac_benchmarks() { 

    cd pthreads/${CLOCK}

    PTNT=$NUM_THREADS

    # Bind Settings --> Mac cannot do binding [from my hours of searching if you can]

    # Schedule Settings
    # Other Settings

    if (( $NUM_THREADS < $ITERS )); then # !!!!
        PTNT=$ITERS
    fi

    # COMPILE
    make all

    # GATHER EXECUTABLES & EXECUTE
    for((i=0;i<($RUNS);i++)); 
	do
        for f in ./*; do
            if [[ -x "$f" && "${FN}" != "Makefile" ]]; then
                FN="$(basename -- "$f")"
                PTHREADS_DATA="data/${FN}_${PTNT}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_x_x_x.csv"
                if [ ! -d "$f" ]; then
                    if [ ! -f ${PTHREADS_DATA} ]; then
                        touch ${PTHREADS_DATA}
                    fi
                    ./${f} $PTNT $ITERS $PT_BIND $PT_PLACES $PT_MASTER >> ${PTHREADS_DATA}
                fi
            fi
        done
    done

    LANG="pthreads"

    # METRICS
    for f in data/*; do
        if [[ ! -d "$f" ]]; then
            PATTERN="^data/[A-Z0-9]+_${PTNT}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_x_x_x\.csv$"
            if [[ "$f" =~ $PATTERN ]]; then
                # file , nt , lang , runs , iters , clock , fcn , [bind settings] , [other settings] , arch
                python3 ./../../../../scripts/process_metrics.py ${f} $PTNT $LANG $RUNS $ITERS $CLOCK $FCN "[x,x,x]" "[]" $ARCH >> ../../../../scripts/${OUTPUT}
            fi
        fi
    done


    # CLEAN
    make clean &>/dev/null

    # exit into mac folder 
    cd ../..
}
serial_benchmarks() {
    cd serial/${CLOCK}

    # COMPILE
    make $1

    # GATHER EXECUTABLES & EXECUTE
    for((i=0;i<($RUNS);i++)); 
	do
        for f in ./*; do
            if [[ -x "$f" && "${FN}" != "Makefile" ]]; then
                FN="$(basename -- "$f")"
                SERIAL_DATA="data/${FN}_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}.csv"
                if [ ! -d "$f" ]; then
                    if [ ! -f ${SERIAL_DATA} ]; then
                        touch ${SERIAL_DATA}
                    fi
                    ./${f} $NUM_THREADS $ITERS >> ${SERIAL_DATA}
                fi
            fi
        done
    done

    LANG="serial"

    # METRICS
    for f in data/*; do
        if [[ ! -d "$f" ]]; then
            PATTERN="^data/[A-Z0-9]+_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}\.csv$"
            if [[ "$f" =~ $PATTERN ]]; then
                # file , nt , lang , runs , iters , clock , fcn , [bind settings] , [other settings] , arch
                python3 ./../../../../scripts/process_metrics.py ${f} $NUM_THREADS $LANG $RUNS $ITERS $CLOCK $FCN "[]" "[]" $ARCH >> ../../../../scripts/${OUTPUT}
            fi
        fi
    done

    # CLEAN
    make clean &>/dev/null

    cd ../..
}

#### #### RUN BENCHMARKS #### ####

OUTPUT="output/001_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_${OMP_BIND}_${OMP_PLACES}_${PT_BIND}_${PT_MASTER}.csv"
if [ -f ${OUTPUT} ]; then
    rm ${OUTPUT}
fi
touch ${OUTPUT}

cd ../benchmarks/

if (( $LINUX == 0 )); then
    cd mac/
    rm pthreads/${CLOCK}/data/0*_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_x_x_x.csv
    if (( $NUM_THREADS < $ITERS )); then # !!!!
        rm pthreads/${CLOCK}/data/0*_${ITERS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_x_x_x.csv
    fi
    pthreads_mac_benchmarks
    cd ..
fi

cd afterspawn/

    # cilk
    rm cilk/${CLOCK}/data/01*_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}.csv
    cilk_benchmarks 1

    # openmp
    rm openmp/${CLOCK}/data/01*_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_${OMP_BIND}_${OMP_PLACES}.csv
    openmp_benchmarks 1 # makefile# executablename ?

    # pthreads - Linux
    if (( $LINUX == 1)); then
        rm pthreads/${CLOCK}/data/01*_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_${PT_BIND}_${PT_PLACES}_${PT_MASTER}.csv
        if (( $NUM_THREADS < $ITERS )); then # !!!!
            rm pthreads/${CLOCK}/data/01*_${ITERS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_${PT_BIND}_${PT_PLACES}_${PT_MASTER}.csv
        fi
        pthreads_benchmarks 1
    fi

    # serial
    rm serial/${CLOCK}/data/01*_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}.csv
    serial_benchmarks 1

cd ..

cd afterspawnsync/

    # cilk
    rm cilk/${CLOCK}/data/01*_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}.csv
    cilk_benchmarks 1

    # openmp
    rm openmp/${CLOCK}/data/01*_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_${OMP_BIND}_${OMP_PLACES}.csv
    openmp_benchmarks 1 # makefile# executablename ?

    # pthreads
    if (( $LINUX == 1)); then
        rm pthreads/${CLOCK}/data/01*_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_${PT_BIND}_${PT_PLACES}_${PT_MASTER}.csv
        if (( $NUM_THREADS < $ITERS )); then # !!!!
            rm pthreads/${CLOCK}/data/01*_${ITERS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_${PT_BIND}_${PT_PLACES}_${PT_MASTER}.csv
        fi    
        pthreads_benchmarks 1
    fi

cd ..

cd beforespawn/

    # cilk
    rm cilk/${CLOCK}/data/02*_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}.csv
    cilk_benchmarks 2

    # openmp
    rm openmp/${CLOCK}/data/02*_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_${OMP_BIND}_${OMP_PLACES}.csv
    openmp_benchmarks 2 # makefile# executablename ?

    # pthreads
    if (( $LINUX == 1)); then
        rm pthreads/${CLOCK}/data/02*_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_${PT_BIND}_${PT_PLACES}_${PT_MASTER}.csv
        if (( $NUM_THREADS < $ITERS )); then # !!!!
            rm pthreads/${CLOCK}/data/02*_${ITERS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_${PT_BIND}_${PT_PLACES}_${PT_MASTER}.csv
        fi    
        pthreads_benchmarks 2
    fi

    # serial
    rm serial/${CLOCK}/data/02*_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}.csv
    serial_benchmarks 2

cd ..

cd returnspawn/

    # cilk
    rm cilk/${CLOCK}/data/03*_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}.csv
    cilk_benchmarks 3

    # openmp
    rm openmp/${CLOCK}/data/03*_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_${OMP_BIND}_${OMP_PLACES}.csv
    openmp_benchmarks 3 # makefile# executablename ?

    # pthreads
    if (( $LINUX == 1)); then
        rm pthreads/${CLOCK}/data/03*_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_${PT_BIND}_${PT_PLACES}_${PT_MASTER}.csv
        if (( $NUM_THREADS < $ITERS )); then # !!!!
            rm pthreads/${CLOCK}/data/03*_${ITERS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_${PT_BIND}_${PT_PLACES}_${PT_MASTER}.csv
        fi
        pthreads_benchmarks 3
    fi

    # serial
    rm serial/${CLOCK}/data/03*_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}.csv
    serial_benchmarks 3

cd ..

cd startspawnfcn/

    # cilk
    rm cilk/${CLOCK}/data/04*_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}.csv
    cilk_benchmarks 4

    # openmp
    rm openmp/${CLOCK}/data/04*_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_${OMP_BIND}_${OMP_PLACES}.csv
    openmp_benchmarks 4 # makefile# executablename ?

    # pthreads
    if (( $LINUX == 1)); then
        rm pthreads/${CLOCK}/data/04*_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_${PT_BIND}_${PT_PLACES}_${PT_MASTER}.csv
        if (( $NUM_THREADS < $ITERS )); then # !!!!
            rm pthreads/${CLOCK}/data/04*_${ITERS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_${PT_BIND}_${PT_PLACES}_${PT_MASTER}.csv
        fi    
        pthreads_benchmarks 4
    fi

    # serial
    rm serial/${CLOCK}/data/04*_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}.csv
    serial_benchmarks 4

cd ..

cd spawntree/

    # cilk
    rm cilk/${CLOCK}/data/06*_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}.csv
    cilk_benchmarks 6

    # openmp
    rm openmp/${CLOCK}/data/06*_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_${OMP_BIND}_${OMP_PLACES}.csv
    openmp_benchmarks 6 # makefile# executablename ?

    # pthreads
    if (( $LINUX == 1)); then
        rm pthreads/${CLOCK}/data/06*_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_${PT_BIND}_${PT_PLACES}_${PT_MASTER}.csv
        if (( $NUM_THREADS < $ITERS )); then 
            rm pthreads/${CLOCK}/data/06*_${ITERS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}_${PT_BIND}_${PT_PLACES}_${PT_MASTER}.csv
        fi
        pthreads_benchmarks 6
    fi

    # serial
    rm serial/${CLOCK}/data/06*_${NUM_THREADS}_${RUNS}_${ITERS}_${CLOCK}_${FCN}.csv
    serial_benchmarks 6

cd ..

cd ../scripts/


#### #### #### #### #### ####

#### #### METRICS #### ####

# dire: ./runall.sh -r 20 -i 32 -nt 32 >> output/vanilla_run_000.txt
# Then: ./runall.sh -r 20 -i 32 -nt 32 -pmpt 8 -bomp spread -pomp cores >> output/bind_run_000.txt
#                                                                           PT bind master to core 8, OMP bind spread at core-level
# copy/paste to excel and do the math / visuals

# min,median,max
# med/min
# max/min


#### #### #### #### #### ####

#### #### OUTPUT OPTIONS #### ####



#### #### #### #### #### ####