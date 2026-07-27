#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <omp.h>
#include <assert.h>
#include <math.h>
#include <sys/time.h>
#include "../../../include/numthreads.h"
#include "../../../include/ctimer.h"
#include <time.h>

/* Benchmark: 03F: Spawn time beforefunc ; OneArgs Spawn (OpenMP)
 * Launch a bunch and measure when all done 
 */
static const int ITERATION = 100000;
struct timespec spawn_function_long(double x){

    double z = 0;
    double i = 0.0;

    // double x = 15.0;
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

    // double t_start = 0.0;
    // t_start = omp_get_wtime();

    struct timespec t_start;
	clock_gettime(CLOCK_MONOTONIC, &t_start);

    // struct timeval t_start;
    // gettimeofday(&t_start, NULL);

	return t_start;
}
struct timespec spawn_function(int x){           // Simple Function to Spawn

    // int x = 100; 
    int y = 5000; int z = 1000000;

	x = x + y + z;

	y = y + x + z;

	z = z + y + x;	

    // double t_start = 0.0;
    // t_start = omp_get_wtime();

    struct timespec t_start;
	clock_gettime(CLOCK_MONOTONIC, &t_start);

    // struct timeval t_start;
    // gettimeofday(&t_start, NULL);

	return t_start; 
}

int main(int argc, char *argv[]){

    int OMP_THREADS = args_threads(argc, argv);

    // int iters=iter_args(argc, argv);

    double x = 15.0;

	struct timespec t_start, t_res, t_end;

	#pragma omp parallel num_threads(OMP_THREADS) 
	{
		#pragma omp master //single
		{		
				#pragma omp task	
				t_start = spawn_function(x);//_long(x); 

                clock_gettime(CLOCK_MONOTONIC, &t_end);
		}
	}
    

	// printf("****\n");
    timespec_sub(&t_res, t_end, t_start);
	if(t_res.tv_nsec < 0 && t_res.tv_sec >= 0){ t_res.tv_nsec *= -1; }// ABS VALUE printf("-");}
    printf("%ld.%09ld,03F,openmp\n", (long)t_res.tv_sec, t_res.tv_nsec);

	return 0;
}