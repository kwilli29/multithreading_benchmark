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
/* Benchmark: 03E: Spawn time return; One Spawns (Pthreads)
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
    // gettimeofday((struct timeval *)t_start, NULL);

	return (void*)t_start;
}
void* spawn_function(void* t_start){           // Simple Math for Spawn Function

	int x = 100; int y = 5000; int z = 1000000;

	x = x + y + z;

	y = y + x + z;

	z = z + y + x;

    clock_gettime(CLOCK_MONOTONIC, (struct timespec *)t_start);
    // gettimeofday((struct timeval *)t_start, NULL);

	return (void*)t_start; 
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
    if( argc >= 4 && ((argv[3][0] != 'x') || (argv[4][0] != 'x') || (argv[5][0] != 'x')))  { attr= thread_affinity(argv); attr_flag=1;}

    // cpu_num = sched_getcpu();
    // printf("MASTER Thread (B) is running on CPU %3d\n", cpu_num);

	struct timespec t_start, t_end, t_res;

	/****/ 

    if(attr_flag){
    	// pthread_create( &Thread[0], &attr, spawn_function_long, (void*)&t_start);
        pthread_create( &Thread[0], &attr, spawn_function, (void*)&t_start);
    } else{
        // pthread_create( &Thread[0], NULL, spawn_function_long, (void*)&t_start);
        pthread_create( &Thread[0], NULL, spawn_function, (void*)&t_start);
    }

	struct timespec* temp = (struct timespec *)&t_start;

	pthread_join( Thread[0], (void*)&temp); // join

    clock_gettime(CLOCK_MONOTONIC, &t_end);
    timespec_sub(&t_res, t_end, t_start);

    if(t_res.tv_nsec < 0 && t_res.tv_sec >= 0){ t_res.tv_nsec *= -1; }// ABS VALUE printf("-");}

    printf("%ld.%09ld,03E,pthreads\n", (long)t_res.tv_sec, t_res.tv_nsec);	

    if(attr_flag){ pthread_attr_destroy(&attr); }

	return 0;
}