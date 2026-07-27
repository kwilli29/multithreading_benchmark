#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <assert.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>
#include "../../../include/numthreads.h"
#include "../../../include/ctimer.h"

/* Benchmark: 02D: Spawn time before ; CilkFor Spawns (Cilk)
 * Launch a bunch and measure when all done - don’t necessarily get just spawn time
 */
static const int ITERATION = 100000;
void spawn_function_long(){

    double z = 0;
    double i = 0.0;

    double x = 15.0;
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
void spawn_function(){           // Simple Function to Spawn

	int x = 100; int y = 5000; int z = 1000000;

	x = x + y + z;

	y = y + x + z;

	z = z + y + x;	

	return; 
}

int main(int argc, char *argv[]){
    
    int iter = iter_args(argc, argv);

	struct timespec t_start, t_res;
	struct timespec t_end[iter];
	clock_gettime(CLOCK_MONOTONIC, &t_start); // struct timespec *tp


	#pragma cilk grainsize 1
	cilk_for(int i=0; i < iter; i++){ // parallel for loop time then spawn
		
		clock_gettime(CLOCK_MONOTONIC, &t_end[i]); 
        
        cilk_spawn spawn_function();
        //cilk_spawn spawn_function_long();

	}

	printf("****,****,****\n");
	for(int i = 0; i < iter; i++){
		
		timespec_sub(&t_res, t_end[i], t_start);
		printf("%ld.%09ld,02D,cilk\n", (long)t_res.tv_sec, t_res.tv_nsec);

	
	}

	// printf("02D\n");

	return 0;
}