#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <math.h>
#include <sys/time.h>
#include "../../include/numthreads.h"
#include "../../include/ctimer.h"
#include <time.h>

/* Benchmark: 04F: Spawn time before function ; One Spawn w/ fcn arg (Pthreads)
 * Launch a bunch and measure when all done
 */

static struct timespec t_end; 

static const int ITERATION = 100000;
void* spawn_function_long(void* x){

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
            *(double *)x = sin(*(double *)x*1.0) / (double)(a*1.0 + (j * i + i + j)*1.0 / a);
        }

        z += *(double *)x + z; //
        z= tanh((double)z);

        i += 1.0;
	}

	return (void*)&t_end;
}
void* spawn_function(void* x){           // Simple Math for Spawn Function

	clock_gettime(CLOCK_MONOTONIC, &t_end);

	int y = 5000; int z = 1000000;

	*(int *)x = *(int *)x + y + z;

	y = y + *(int *)x + z;

	z = z + y + *(int *)x;	

	return (void*)&t_end; 
}

int main(int argc, char *argv[]){

    // int cpu_num = sched_getcpu();
    // printf("MASTER Thread (A) is running on CPU %3d\n", cpu_num);

	int PTH = args_threads(argc, argv);

    int iters=iter_args(argc, argv);
    if( PTH < iters ){ PTH = iters; }
    
	pthread_t Thread[ PTH ];

    bool attr_flag = 0;
    pthread_attr_t attr;
    if( argc >= 4 && ((argv[3][0] != 'x') || (argv[4][0] != 'x') || (argv[5][0] != 'x')))  { 
        // attr= thread_affinity(argv); 
        attr_flag=0;
    }

    // cpu_num = sched_getcpu();
    // printf("MASTER Thread (B) is running on CPU %3d\n", cpu_num);

    /****/
    
	double x = 15.0;
	double* xp= &x;

	/****/   

	struct timespec t_start, t_res;
	clock_gettime(CLOCK_MONOTONIC, &t_start);

    if(attr_flag){ 
        // pthread_create( &Thread, &attr, spawn_function_long, xp);
        pthread_create( &Thread[0], &attr, spawn_function, xp);
    } else {
        // pthread_create( &Thread, NULL, spawn_function_long, xp);
        pthread_create( &Thread[0], NULL, spawn_function, xp);
    }

	struct timespec* temp = &t_end;
	pthread_join(Thread[0], (void *)&temp);

	timespec_sub(&t_res, t_end, t_start);
	printf("%ld.%09ld,04F,pthreads\n", (long)t_res.tv_sec, t_res.tv_nsec);
     		
    if(attr_flag){ pthread_attr_destroy(&attr); }

	return 0;
}