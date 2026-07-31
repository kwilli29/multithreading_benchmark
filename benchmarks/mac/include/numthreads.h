
#ifndef NUMTHREADS_H_   /* Include guard */
#define NUMTHREADS_H_

#define buffersize 128

int number_threads();
// pthread_attr_t thread_affinity(int id);
int args_threads(int arg_c, char** arg_v);
int iter_args(int arg_c, char** arg_v);
int detectOS();

#endif // NUMTHREADS_H_