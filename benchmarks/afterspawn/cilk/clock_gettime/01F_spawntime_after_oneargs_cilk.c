#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <cilk/cilk.h>
#include <cilk/cilkscale.h>
#include <cilk/cilk_api.h>
#include <assert.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>
#include "../../../include/numthreads.h"
#include "../../../include/ctimer.h"

/* 
 * Benchmark: 01F: Spawn time after ; One Spawns w/ Fcn args (Cilk)
 * Launch a bunch and measure when all done
 */
static const int ITERATION = 100000;
void spawn_function_long(double x){

    double z = 0;
    double i = 0.0;

    //double x = 15.0;
	static const int nn = 87;

    double a =0.0;
	for (int j = 0; j < ITERATION; j++){
        z*=acos((double)j);

        for (long m = 1; m < nn; ++m){
            a = (double)((double)m*1.0);
            x = sin((double)x*1.0) / (double)(a*1.0 + (j * i + i + j)*1.0 / a);
        }

        z += x + z; //
        z= tanh((double)z);

        i += 1.0;
	}

    // printf("**%d\t", __cilkrts_get_worker_number()); // print thread id

	return;
}

void spawn_function(int x){           // Simple Spawn Function
	int y=5000; int z = 1000000;

	x = x + y + z;

	y = y + x + z;

	z = z + y + x;	

	return; 
}


int main(int argc, char *argv[]){

    int iter = iter_args(argc, argv);
    
	double x = 15.0;

	struct timespec t_start, t_res, t_end;
	clock_gettime(CLOCK_MONOTONIC, &t_start); // struct timespec *tp

	cilk_spawn spawn_function(x); // single spawn with one int arg, can adjust 
    //cilk_spawn spawn_function_long(x);
	
	clock_gettime(CLOCK_MONOTONIC, &t_end);

	cilk_sync;

    timespec_sub(&t_res, t_end, t_start);
	printf("%ld.%09ld,01F,cilk\n", (long)t_res.tv_sec, t_res.tv_nsec);

	// printf("01F\n");

	return 0;
}