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
 
/* Benchmark: 03C: Spawn time return; For-Loop Spawns (Pthreads)
 * Launch a bunch and measure when all done 
 */
static const int ITERATION = 100000;
void* spawn_function_long(void* t_start){

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

    clock_gettime(CLOCK_MONOTONIC, (struct timespec *)t_start);

	return (void*)t_start;
}
void* spawn_function(void* t_start){           // Simple Math for Spawn Function

	int x = 100; int y = 5000; int z = 1000000;

	x = x + y + z;

	y = y + x + z;

	z = z + y + x;	

    clock_gettime(CLOCK_MONOTONIC, (struct timespec *)t_start);

	return (void*)t_start; 
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

	struct timespec t_start[iters]; struct timespec t_res;
	struct timespec t_end[iters];

	/****/ 
    if(attr_flag){
        for( int i = 0; i < iters; i++ ) {                                     // # seq. for only
            // pthread_create( &Threads[ i ], &attr, spawn_function_long, (void*)&t_start[i]);
            pthread_create( &Threads[ i ], &attr, spawn_function, (void*)&t_start[i]);
        }
    } else {
        for( int i = 0; i < iters; i++ ) {                                     // # seq. for only
            // pthread_create( &Threads[ i ], NULL, spawn_function_long, (void*)&t_start[i]);
            pthread_create( &Threads[ i ], NULL, spawn_function, (void*)&t_start[i]);
        }
    }

	struct timespec* temp = (struct timespec *)&t_start[0];
	
	for( int i = 0; i < iters; i++ ) {                                     // join
		pthread_join( Threads[ i ], (void*)&temp);
        
        clock_gettime(CLOCK_MONOTONIC, &t_end[i]);

		if (i < PTH-1){ temp = &t_start[i+1]; }
	}

	for(int i = 0; i < iters; i++){

		timespec_sub(&t_res, t_end[i], t_start[i]);
        if(t_res.tv_nsec < 0 && t_res.tv_sec >= 0){ t_res.tv_nsec *= -1; }// ABS VALUE printf("-");}
		printf("%ld.%09ld,03C,pthreads\n", (long)t_res.tv_sec, t_res.tv_nsec);
    	

	}

    if(attr_flag){ pthread_attr_destroy(&attr); }

	return 0;
}