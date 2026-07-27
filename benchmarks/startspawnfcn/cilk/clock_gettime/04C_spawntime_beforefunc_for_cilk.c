#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <assert.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include "../../../include/numthreads.h"
#include "../../../include/ctimer.h"

/* Benchmark: 04C: Spawn time before thread function begins ; For-Loop Spawns (Cilk) 
 * Launch a bunch and measure when all done
 */
static const int ITERATION = 100000;
struct timespec spawn_function_long(){

	struct timespec t_end; 
	clock_gettime(CLOCK_MONOTONIC, &t_end);

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

	return t_end; // 
}
struct timespec spawn_function(){           // Simple Function to Spawn

	struct timespec t_end; 
	clock_gettime(CLOCK_MONOTONIC, &t_end);

    // struct timeval t_end;
    // gettimeofday(&t_end,NULL);

	int x = 100; int y = 5000; int z = 1000000;

	x = x + y + z;

	y = y + x + z;

	z = z + y + x;	

	return t_end; //  end_time; 
}

int main(int argc, char *argv[]){

    int iter = iter_args(argc, argv);

	struct timespec t_start[iter]; struct timespec t_res; 
	struct timespec t_end[iter];

	// Use for loop, timestamp before spawn to right at start of spawn_function

	for(int i=0; i < iter; i++){ 	
        clock_gettime(CLOCK_MONOTONIC, &t_start[i]); 
        
        t_end[i] = cilk_spawn spawn_function();//_long();

	} 
    
    cilk_sync;
    
	//printf("****\n");	
	for(int i = 0; i < iter; i++){
		
		timespec_sub(&t_res, t_end[i], t_start[i]);
		printf("%ld.%09ld,04C,cilk\n", (long)t_res.tv_sec, t_res.tv_nsec);

	
	}

	// printf("04C\n");
	
	return 0;
}