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

/* Benchmark: 02G Spawn time before ; For-Loop + Join Spawns (Pthreads)
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
    
	pthread_t Threads[ PTH ];

    bool attr_flag = 0;
    pthread_attr_t attr;
    if( argc >= 4 && ((argv[3][0] != 'x') || (argv[4][0] != 'x') || (argv[5][0] != 'x')))  { 
        // attr= thread_affinity(argv); 
        attr_flag=0;
    }

    // cpu_num = sched_getcpu();
    // printf("MASTER Thread (B) is running on CPU %3d\n", cpu_num);

	struct timespec t_start, t_res; struct timespec t_end[iters];
	clock_gettime(CLOCK_MONOTONIC, &t_start);

	/****/ 

    if(attr_flag){
        for( int i = 0; i < iters; i++ ) {                                     // # seq. for only
            clock_gettime(CLOCK_MONOTONIC, &t_end[i]);
            pthread_create( &Threads[ i ], &attr, spawn_function, NULL);//_long, NULL);
        }
    } else {
        for( int i = 0; i < iters; i++ ) {                                     // # seq. for only
            clock_gettime(CLOCK_MONOTONIC, &t_end[i]);
            pthread_create( &Threads[ i ], NULL, spawn_function, NULL);//_long, NULL);
        }
    }

	for( int i = 0; i < iters; i++ ) {                                     // # seq. for only
		pthread_join( Threads[ i ],NULL);
	}

    printf("****,****,****\n");
	for(int i = 0; i < iters; i++){

		timespec_sub(&t_res, t_end[i], t_start);
		printf("%ld.%09ld,02G,pthreads\n", (long)t_res.tv_sec, t_res.tv_nsec);

	}
    if(attr_flag){ pthread_attr_destroy(&attr); }
	return 0;
}
