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

/* Benchmark: 03E: Spawn time return from function ; One Spawn (Cilk) 
 * 
 */
static const int ITERATION = 100000;
struct timespec spawn_function_long(){

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

    struct timespec t_start;
	clock_gettime(CLOCK_MONOTONIC, &t_start);

	return t_start; // 
}
struct timespec spawn_function(){           // Simple Function to Spawn

	int x = 100; int y = 5000; int z = 1000000;

	x = x + y + z;

	y = y + x + z;

	z = z + y + x;	

    struct timespec t_start;
	clock_gettime(CLOCK_MONOTONIC, &t_start);


	return t_start; //  end_time; 
}

int main(int argc, char *argv[]){

    int iter = iter_args(argc, argv);
    
	struct timespec t_start, t_res, t_end;

	// Use for loop, timestamp before spawn to right at start of spawn_function

	t_start = cilk_spawn spawn_function();//_long(); 
    clock_gettime(CLOCK_MONOTONIC, &t_end);

    cilk_sync;
    
	// printf("****\n");	
    timespec_sub(&t_res, t_start, t_end); // t_end happens before t_start
    if(t_res.tv_nsec < 0 && t_res.tv_sec >= 0){ t_res.tv_nsec *= -1; }// ABS VALUE printf("-");}
    printf("%ld.%09ld,03E,cilk\n", (long)t_res.tv_sec, t_res.tv_nsec);

	// printf("03E\n");
	
	return 0;
}