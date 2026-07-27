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

/* Benchmark: 01B: Spawn time after ; Parallel Region Spawns (OpenMP)
 * Launch a bunch and measure when all done
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
void spawn_function(){           // Simple Spawn Function

	int x = 100; int y = 5000; int z = 1000000;

	x = x + y + z;

	y = y + x + z;

	z = z + y + x;	

	return; 
}

int main(int argc, char *argv[]){

	// int OMP_THREADS = args_threads(argc, argv);

    int iters=iter_args(argc, argv);
    
	struct timespec t_start, t_res, t_end;
	clock_gettime(CLOCK_MONOTONIC, &t_start);

	// omp parallel region will call spawn_function in parallel iters # of times

    #pragma omp parallel num_threads(iters) 
    {
    
        spawn_function();
        //spawn_function_long();
        
    }

	clock_gettime(CLOCK_MONOTONIC, &t_end);
	timespec_sub(&t_res, t_end, t_start);

    // !!!! omp_get_proc_bind !!!!
    // int thread_num = omp_get_thread_num();
    // int cpu_num = sched_getcpu();
    // printf("MASTER_END: Thread %3d is running on CPU %3d\n", thread_num, cpu_num);

	printf("%ld.%09ld,01B,openmp\n", (long)t_res.tv_sec, t_res.tv_nsec);

	return 0;
}