#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>
#include <math.h>
#include "../../../include/numthreads.h"
#include "../../../include/ctimer.h"
#include <time.h>

/* Benchmark: 04F: Spawn time beforefunc ; Sequential Spawns (Serial)
 * Launch a bunch and measure when all done - don’t necessarily get just spawn time
 */
static const int ITERATION = 100000;
struct timespec spawn_function_long(double x){

	struct timespec t_end; 						  
	clock_gettime(CLOCK_MONOTONIC, &t_end);


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

	return t_end;
}
struct timespec spawn_function(int x){           // Simple Spawn Function

	struct timespec t_end; 						  
	clock_gettime(CLOCK_MONOTONIC, &t_end);

	int y = 5000; int z = 1000000;

	x = x + y + z;

	y = y + x + z;

	z = z + y + x;	

	return t_end; 
}

int main(int argc, char *argv[]){

    int iter = iter_args(argc, argv);

	double x = 15.0;

	struct timespec t_start, t_res;
	struct timespec t_end;
	clock_gettime(CLOCK_MONOTONIC, &t_start);

    
    t_end = spawn_function(x);//_long(x); // Take time stamp before each spawn

	timespec_sub(&t_res, t_end, t_start);
	printf("%ld.%09ld,04F,serial\n", (long)t_res.tv_sec, t_res.tv_nsec);

	// printf("04F\n");
	
	return 0;
}