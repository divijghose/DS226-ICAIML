# A (gentle) introduction to OpenMP
# Contents
1. [What is OpenMP](#what-is-openmp)
2. [Hello World! (times n)](#hello-world-times-n)
3. [Thread functions](#openmp-thread-functions)
4. [Race Conditions](#race-conditions)\
    a. [Scoping Variables](#scoping-variables)

## What is OpenMP? 
OpenMP (Open Multi-Processing) is an API that provides support for parallel programming in **shared-memory architectures**. It consists of compiler directives which are inserted in the code in regions that can be parallelised, a run-time library to execute these regions in parallel, and environment variables. OpenMP supports parallelism in C, C++ and Fortran, and is preffered for its portability and ease of use.
OpenMP is based on a _thread-based, fork-join model_.

[Back to contents](#contents)

## Hello World! (times n)
Our first task is to print "Hello World!" in parallel. _Parallel_ here refers to parts of the code being executed simultaneously by multiple **threads**.
The first step is to include the OpenMP header file `omp.h`. This contains the prototypes for various OpenMP functions.
```
#include <omp.h>
```
Whenever we want some particular block of the code to be executed in parallel, we include the compiler directive `#pragma omp parallel`.
```
#pragma omp parallel
{
    //block to be executed in parallel
}
```
To compile the code, for the `gcc` compiler , we specify the flag `-fopenmp`. This makes the compiler use the appropriate OpenMP libraries. 
```
gcc -fopenmp ProgramName.c
```
Putting it all together, we get our first OpenMP program!
```
#include <omp.h> // OpenMP Header file
#include <stdio.h>

int main()
{

#pragma omp parallel // Specify the block to be executed in parallel
    {
        printf("Hello World!\n");
    }

    return 0;
}
```
To compile the code, 
```
gcc -fopenmp HelloWorld.c -o HelloWorld.out
```
Then execute,
```
./HelloWorld.out
```
We notice that "Hello World!" is printed each time a thread has executed the print statement. One way to specify the number of threads is to use the environment variable `OMP_NUM_THREADS`
```
export OMP_NUM_THREADS=4
```

[Back to contents](#contents)

## OpenMP thread functions
Each thread can be represented by a thread ID. Knowing the thread ID and total number of threads helps us better understand the distribution of work.
The call `omp_get_num_threads()` return the total number of threads. `omp_get_thread_num()` returns the thread ID of the thread being executed.
```
#include <omp.h> // OpenMP Header file
#include <stdio.h>

int main()
{

#pragma omp parallel // Specify the block to be executed in parallel
    {
        int t_num = omp_get_num_threads(); // Get total number of OMP threads
        int t_id = omp_get_thread_num(); //Get ID of thread being executed
        printf("Hello World from thread %d of %d\n",t_id,t_num);
    }

    return 0;
}
```
Note that each thread will have its own copy of `t_id` and `t_num`.
Compiling and executing the above program,
```
gcc -fopenmp ThreadIDs.c -o ThreadIDs.out
./ThreadIDs.out
```
we get to know exactly which thread is printing on the screen.

When we declare `t_id` and `t_num` outside the parallel block, they become shared variables. This leads to a race condition which may cause errors in the output.
```
#include <omp.h> // OpenMP Header file
#include <stdio.h>

int main()
{
    int t_num, t_id;
#pragma omp parallel // Specify the block to be executed in parallel
    {
        t_num = omp_get_num_threads(); // Get total number of OMP threads
        t_id = omp_get_thread_num();   // Get ID of thread being executed
        printf("Hello World from thread %d of %d\n", t_id, t_num);
    }

    return 0;
}
```
[Back to contents](#contents)
## Race Conditions

It is important to note that a race condition does not occur everytime multiple threads access a shared variable. Instead, a race condition is one in which correctness of the parallel program depends on the order in which the threads are scheduled and the behaviour of the shared variable remains uncertain.
We can ensure that the previous code produces an incorrect result (_almost_) everytime by introducing a time delay before the print function
```
#pragma omp parallel // Specify the block to be executed in parallel
    {
        t_num = omp_get_num_threads(); // Get total number of OMP threads
        t_id = omp_get_thread_num();   // Get ID of thread being executed
        for(j=0;j<10000000;j++); // Introduce time delay to make code fail
        printf("Hello World from thread %d of %d\n", t_id, t_num);
    }
```
### Scoping variables
We can prevent the race condition in the last example by declaring `t_id` as `private`.
```
int t_num, t_id;
#pragma omp parallel private(t_id) shared(t_num)
```

[Back to contents](#contents)