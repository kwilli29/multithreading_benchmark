#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>
#include <sched.h>
#include "ctimer.h"

#include "numthreads.h"

// run valgrind !!

// gcc -O3 -Og -g -Wall -Wconversion -lm numthreads.c // ./a.out

int linux_threads(){
    // Parse lscpu

    FILE *fp;
    char buffer[buffersize];
    int NUM_THREADS = (int)2; // Default to 2 if command fails or parsing fails

    // Open the pipe to the command
    fp = popen("lscpu | grep '^CPU(s):'", "r");


    if (fp != NULL) {

        // Read the output line (e.g., "CPU(s):                          NUM_THREADS")
        if (fgets(buffer, sizeof(buffer), fp) != NULL) {
            // Find the colon and scan the integer following it 
            if (sscanf(buffer, "CPU(s): %d", &NUM_THREADS) != 1) {
                // If parsing fails, reset to 2 
                NUM_THREADS = (int)2;
            }
        }
        
        // Close the pipe and get exit status
        if (pclose(fp) != 0) {
            NUM_THREADS = (int)2;
        }
    }

    //printf("%d\n", (int)NUM_THREADS);

    return (int)NUM_THREADS;
}

int mac_threads(){

    FILE *fp;
    char buffer[buffersize];
    int NUM_THREADS = (int)2; // Default to 2 if command fails or parsing fails

    // Open the pipe to the command
    fp = popen("sysctl -a | grep '^machdep.cpu.thread_count:'", "r");

    // Mac needs to run "sysctl -a | grep machdep.cpu" and get 'machdep.cpu.thread_count: #'
    
    if (fp != NULL) {

        // Read the output line (e.g., "machdep.cpu.thread_count: 10")
        if (fgets(buffer, sizeof(buffer), fp) != NULL) {
            // Find the colon and scan the integer following it 
            if (sscanf(buffer, "machdep.cpu.thread_count: %d", &NUM_THREADS) != 1) {
                // If parsing fails, reset to 2 
                NUM_THREADS = (int)2;
            }
        }
        
        // Close the pipe and get exit status
        if (pclose(fp) != 0) {
            NUM_THREADS = (int)2;
        }
    }

    //printf("%d\n", (int)NUM_THREADS);

    return (int)NUM_THREADS;
}

int number_threads() {
    FILE *fp;
    char buffer[buffersize];
    char os_buffer[256];

    int NUM_THREADS = (int)2; // Default to 2 if command fails or parsing fails

    // Windows needs something different that would fail on the other os options probably

    // Open the pipe to the command
    fp = popen("uname -s", "r");
    
    if (fp != NULL) {

        // Read the output line (e.g., "Linux")
        if (fgets(buffer, sizeof(buffer), fp) != NULL) {
            // Find string from the uname command
            if (sscanf(buffer, "%s", (char *)&os_buffer) != 1) {

                // If parsing fails, OS = other 
                strcpy(os_buffer, "other");

            }

            // printf("%s\n", os_buffer);

            if (strstr((char *)&os_buffer, "Darwin") != NULL){
                NUM_THREADS = mac_threads();
                //strcpy(os_buffer, "mac");
            } 
            else if (strstr((char *)&os_buffer, "Linux") != NULL){
                NUM_THREADS = linux_threads();
                //strcpy(os_buffer, "linux");
            }
            else{
                // Windows or other OS
            }
        }
        
        // Close the pipe and get exit status
        if (pclose(fp) != 0) {
            NUM_THREADS = (int)2;
            strcpy(os_buffer, "other");
        }
    }

    // printf("%d\n", (int)NUM_THREADS);

    return (int)NUM_THREADS;
}

int args_threads(int arg_c, char** arg_v){

	int threads = 32;

    // Process Command-Line Arguments
    if(arg_c >= 2){
        if(atoi(arg_v[1]) == 0){
            threads = number_threads();
        } else {
            threads = atoi(arg_v[1]);
            if (threads < 1){
                threads = number_threads();
            }
        }
    }
	// printf("* # Spawns: %d\n", threads);

    return threads;
}

int iter_args(int arg_c, char** arg_v) {

    int iters= 50;
    if (arg_c >= 3){ iters = atoi(arg_v[2]); }

    return iters;

}

void spawn_function_long_include(){

    static const int ITERATION = 100000;

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
void spawn_function_include(){           // Simple Spawn Function

	int x = 100; int y = 5000; int z = 1000000;

	x = x + y + z;

	y = y + x + z;

	z = z + y + x;	

	return; 
}

/*
int main(int argc, char *argv[]){
    int NUM_THREADS = number_threads();
    printf("%d\n", NUM_THREADS);
    return 0;
}
*/