#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <sys/time.h>
#include "../../../include/numthreads.h"
#include "../../../include/ctimer.h"
#include <time.h>

/* Benchmark: 03C: Spawn time return ; For-Loop Spawns (Serial) 
 * Launch a bunch and measure when all done - don’t necessarily get just spawn time
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

    struct timespec t_start; 						  
	clock_gettime(CLOCK_MONOTONIC, &t_start);

    // struct timeval t_start;
    // gettimeofday(&t_start, NULL);

	return t_start;
}
struct timespec spawn_function(){           // Simple Function to Spawn

	int x = 100; 
    int y = 5000; int z = 1000000;

	x = x + y + z;

	y = y + x + z;

	z = z + y + x;	

    struct timespec t_start; 						  
	clock_gettime(CLOCK_MONOTONIC, &t_start);

    // struct timeval t_start;
    // gettimeofday(&t_start, NULL);

	return t_start; 
}

int main(int argc, char *argv[]){

    int iters = iter_args(argc, argv);
    
	struct timespec t_start[iters]; struct timespec t_res; 
	struct timespec t_end[iters];

	for(int i=0; i < iters; i++){ 	
        t_start[i] = spawn_function(); //_long(); 
        clock_gettime(CLOCK_MONOTONIC, &t_end[i]); 

	} 

	// printf("****\n");	
	for(int i = 0; i < iters; i++){
		
		timespec_sub(&t_res, t_end[i], t_start[i]);
        if(t_res.tv_nsec < 0 && t_res.tv_sec >= 0){ t_res.tv_nsec *= -1; }// ABS VALUE printf("-");}
		printf("%ld.%09ld,03C,serial\n", (long)t_res.tv_sec, t_res.tv_nsec);
                    

	
	}

	// printf("03C\n");
	
	return 0;
}