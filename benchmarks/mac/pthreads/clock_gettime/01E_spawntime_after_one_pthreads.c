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

/* Benchmark: 01E: Spawn timer after ; One Spawn (Pthreads)
 * Launch a bunch and measure when all done 
 */
static const int ITERATION = 100000;
void* spawn_function_long(){

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

	return (void*) NULL; 
}
void* spawn_function(){           // Simple Math for Spawn Function

    // int cpu_num = sched_getcpu();
    // printf(",,,CHILDSPAWN: Thread is running on CPU %3d\n", cpu_num);

	int x = 100; int y = 5000; int z = 1000000;

	x = x + y + z;

	y = y + x + z;

	z = z + y + x;	

	return (void*) NULL; 
}

int main(int argc, char *argv[]){

    // int cpu_num = sched_getcpu();
    // printf("MASTER Thread (A) is running on CPU %3d\n", cpu_num);

	int PTH = args_threads(argc, argv);

    int iters=iter_args(argc, argv);
    if( PTH < iters ){ PTH = iters; }
    
	int ds, rc;
    pthread_attr_t attr;

	if( argc >= 4 && ((argv[3][0] != 'x') || (argv[4][0] != 'x') || (argv[5][0] != 'x')))  { 
        // attr= thread_affinity(argv); 
    }
    else {
        rc = pthread_attr_init(&attr);
        if (rc == -1) { perror("error in pthread_attr_init"); exit(1); }
    }

    // cpu_num = sched_getcpu();
    // printf("MASTER Thread (B) is running on CPU %3d\n", cpu_num);

	ds = 1; // detached
	rc = pthread_attr_setdetachstate(&attr, ds);
	if (rc == -1) { perror("error in pthread_attr_setdetachstate"); exit(2); }

	pthread_t Thread[ PTH ];
    
	struct timespec t_start, t_res, t_end;
	clock_gettime(CLOCK_MONOTONIC, &t_start);	

	/****/ 

	pthread_create( &Thread[0], &attr, spawn_function, NULL);//_long, NULL); // create a single detached thread

	clock_gettime(CLOCK_MONOTONIC, &t_end);

	// destroy attr
	pthread_attr_destroy(&attr);
	
    // cpu_num = sched_getcpu();
    // printf("MASTER Thread (C) is running on CPU %3d\n", cpu_num);

	timespec_sub(&t_res, t_end, t_start);
	printf("%ld.%09ld,01E,pthreads\n", (long)t_res.tv_sec, t_res.tv_nsec);


	return 0;
}