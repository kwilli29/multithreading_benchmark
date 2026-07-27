#!/bin/bash

ARCH=galahad
DATA="data/${ARCH}/PT_BINDTEST.txt"

## PTHREADS ##

pt_places() {

    cd ../benchmarks/afterspawn/pthreads/clock_gettime/
    touch $DATA     


    make "$2" # Number
	echo "$1$2$3" # Letter
    CURRPROG="$1$2$3"

    RUNS=5
    NT=64

    for((i=0;i<($RUNS);i++)); 
    do	
        # for loop
        echo ",Bind kids thread to core i=,0" >> $DATA
        ./$CURRPROG $NT 0 >> $DATA

        echo ",Bind kids thread to core i=,50" >> $DATA
        ./$CURRPROG $NT 50 >> $DATA

        echo ",Bind kids thread to core i=,271" >> $DATA
        ./$CURRPROG $NT 271 >> $DATA

    done

    make clean

    cd ../../../../scripts/
}


# rm $DATA

# Bind Kids to same core: 0, 50, 271
pt_places 0 1 C
pt_places 0 1 N
pt_places 0 1 E
pt_places 0 1 F

pt_places 0 2 C
pt_places 0 2 G

pt_places 0 3 C
pt_places 0 3 E
pt_places 0 3 F

pt_places 0 4 C
pt_places 0 4 E
pt_places 0 4 F

pt_places 0 6 A

# Bind Kids to diff. cores, but close:

