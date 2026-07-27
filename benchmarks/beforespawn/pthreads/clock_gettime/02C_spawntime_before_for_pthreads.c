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
#include "../../../include/numthreads.h"
#include "../../../include/ctimer.h"
#include <time.h>
pthread_attr_t thread_affinity(char** arg_v){

    int pt_bind_core = -1;
    // int pt_place_core = -1;
    int pt_master_core = -1;

    // Create a cpuset
    cpu_set_t cpuset;
    // Clear the cpuset
    CPU_ZERO(&cpuset);
    
    pthread_attr_t attr;

    // Initialize attributes and CPU set
    int rc = pthread_attr_init(&attr);
    if (rc == -1) { perror("error in pthread_attr_init"); exit(1); }

    // Bind Future Spawned Threads
    if(arg_v[3][0] != 'x'){             // Bind Core
        pt_bind_core = atoi(arg_v[3]);
        // add cpu core to the cpuset
        CPU_SET(pt_bind_core, &cpuset);
        // Apply the CPU set to the attribute structure
        pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
    }
    if(arg_v[4][0] != 'x'){             // Places: TBD
        // pt_place_core = atoi(arg_v[4]);
    }

    // Bind Master Thread
    if(arg_v[5][0] != 'x'){
        pt_master_core = atoi(arg_v[5]);
        // add cpu core to the cpuset
        CPU_SET(pt_master_core, &cpuset);
        pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    }

    return attr;

} 
/* Benchmark: 02C Spawn time before ; For-Loop Spawns (Pthreads)
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

	// pthread_barrier_wait 
	//pthread_barrier_wait(&sync_barrier);

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

	if( argc >= 4 && ((argv[3][0] != 'x') || (argv[4][0] != 'x') || (argv[5][0] != 'x'))) { attr= thread_affinity(argv); }
    else {
        rc = pthread_attr_init(&attr);
        if (rc == -1) { perror("error in pthread_attr_init"); exit(1); }
    }

    // cpu_num = sched_getcpu();
    // printf("MASTER Thread (B) is running on CPU %3d\n", cpu_num);

	ds = 1;
	rc = pthread_attr_setdetachstate(&attr, ds);
	if (rc == -1) { perror("error in pthread_attr_setdetachstate"); exit(2); }

	pthread_t Threads[ PTH ];

	struct timespec t_start, t_res;
	struct timespec t_end[iters];
	clock_gettime(CLOCK_MONOTONIC, &t_start);

	/****/ 

	for( int i = 0; i < iters; i++ ) {                                     // # seq. for only
		clock_gettime(CLOCK_MONOTONIC, &t_end[i]);

		pthread_create( &Threads[ i ], &attr, spawn_function, NULL); //_long, NULL);
	}


	// destroy attr
	pthread_attr_destroy(&attr);


	printf("****,****,****\n");	
	for(int i = 0; i < iters; i++){

		timespec_sub(&t_res, t_end[i], t_start);
		printf("%ld.%09ld,02C,pthreads\n", (long)t_res.tv_sec, t_res.tv_nsec);

	}

	return 0;
}