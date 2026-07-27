#define _GNU_SOURCE
#include <sched.h>

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
#include "../../include/numthreads.h"
#include <time.h>
enum {
    _MSEC_PER_SEC = 1000,
    _USEC_PER_SEC = 1000 * 1000,
    _NSEC_PER_SEC = 1000 * 1000 * 1000
};
void timespec_sub( struct timespec * t_diff, struct timespec const  t_end,  struct timespec const  t_start ) {
    /**<[out] time difference */ /**<[in]  end time */ /**<[in]  start time */
    t_diff->tv_nsec = t_end.tv_nsec - t_start.tv_nsec; t_diff->tv_sec  = t_end.tv_sec  - t_start.tv_sec;
    if ((t_diff->tv_sec > 0) && (t_diff->tv_nsec < 0)) {
        t_diff->tv_nsec += _NSEC_PER_SEC; t_diff->tv_sec--;
    } else if ((t_diff->tv_sec < 0) && (t_diff->tv_nsec > 0)) {
        t_diff->tv_nsec -= _NSEC_PER_SEC; t_diff->tv_sec++;
    }
    /* (s > 0 & ns > 0) : do nothing (t_start < t_end) */ /* (s < 0 & ns < 0) : do nothing (t_start > t_end) */
}
/* Benchmark: 01E: Spawn time after ; One Spawns (OpenMP)
 * Launch a bunch and measure when all done 
 */

void spawn_function(){           // Simple Spawn Function

    // !!!! omp_get_proc_bind !!!!
    int thread_num = omp_get_thread_num();
    int cpu_num = sched_getcpu();
    printf("START: Thread %3d is running on CPU %3d\n", thread_num, cpu_num);

	int x = 100; int y = 5000; int z = 1000000;

	x = x + y + z;

	y = y + x + z;

	z = z + y + x;	


    // thread_num = omp_get_thread_num();
    // cpu_num = sched_getcpu();
    // printf("END: Thread %3d is running on CPU %3d\n", thread_num, cpu_num);

	return; 
}

int main(int argc, char *argv[]){

	int OMP_THREADS = number_threads();

    // Process Command-Line Arguments
    if(argc >= 2){
        if(atoi(argv[1]) == 0){
            OMP_THREADS = number_threads();
        } else {
            OMP_THREADS = atoi(argv[1]);
            if (OMP_THREADS < 1){
                OMP_THREADS = number_threads();;
            }
        }
    }

	struct timespec t_start, t_res, t_end;
	clock_gettime(CLOCK_MONOTONIC, &t_start); // struct timespec *tp

	// I really think this is the most representative way to spawn 1 "thread" in a pool of 200
	// Not really what openmp is for

    int thread_num = omp_get_thread_num();
    int cpu_num = sched_getcpu();
    printf("MASTER_START: Thread %3d is running on CPU %3d\n", thread_num, cpu_num);

	#pragma omp parallel num_threads(OMP_THREADS) //num_threads(1) 
	{
		#pragma omp task
		spawn_function();

	}	


	clock_gettime(CLOCK_MONOTONIC, &t_end);
	timespec_sub(&t_res, t_end, t_start);

    //
    thread_num = omp_get_thread_num();
    cpu_num = sched_getcpu();
    printf("MASTER_END: Thread %3d is running on CPU %3d\n", thread_num, cpu_num);
    //
    
	printf("%ld.%09ld\n", (long)t_res.tv_sec, t_res.tv_nsec);


	return 0;
}