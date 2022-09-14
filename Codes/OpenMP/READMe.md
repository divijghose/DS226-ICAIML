# A (gentle) introduction to OpenMP
# Contents
1. [What is OpenMP](#what-is-openmp)
2. [Hello World! (times n)](#hello-world-times-n)
3. [Thread functions](#openmp-thread-functions)
4. [Race Conditions](#race-conditions)\
    a. [Scoping Variables](#scoping-variables)
5. [Thread Synchronization](#thread-synchronization)\
    a. [Fork-Join Model - An Example](#fork-join-model--an-example)\
    b. [Thread Private Variables](#thread-private-variables)\
    c. [#pragma omp barrier](#pragma-omp-barrier)\
    d. [#pragma omp single](#pragma-omp-single)\
    e. [#pragma omp master](#pragma-omp-master)

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
The call `omp_get_num_threads()` returns the total number of threads. `omp_get_thread_num()` returns the thread ID of the thread being executed.
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
We can prevent the race condition in the last example by declaring `t_id` as `private`, as implemented in `ThreadIDs_Race_Scoping.c`
```
int t_num, t_id;
#pragma omp parallel private(t_id) shared(t_num)
```

[Back to contents](#contents)
## Thread Synchronization

### Fork-Join Model : An example

Since `t_num` is the same for all threads, we might be tempted to initialize it outside the parallel region to avoid repeating the initialization for each thread. The resulting error, in `ForkJoin.c`, illustrates the fork-join model.
```
#include <omp.h> // OpenMP Header file
#include <stdio.h>

int main()
{
    int t_num = omp_get_num_threads(); // Get total number of OMP threads

#pragma omp parallel // Specify the block to be executed in parallel
    {
        int t_id = omp_get_thread_num(); // Get ID of thread being executed
        printf("Hello World from thread %d of %d\n", t_id, t_num);
    }

    return 0;
}
```
We can correct this, as seen in `ForkJoin_Corrected.c` by initializing `t_num` in a separate parallel block first, and then using it in the next parallel block.
```
#include <omp.h> // OpenMP Header file
#include <stdio.h>

int main()
{
    int t_num;
#pragma omp parallel // Specify the block to be executed in parallel
    {
        int t_id = omp_get_thread_num(); // Get ID of thread being executed
        if (t_id == 0)
            t_num = omp_get_num_threads(); // Get total number of OMP threads
    }

#pragma omp parallel // Specify the block to be executed in parallel
    {
        int t_id = omp_get_thread_num(); // Get ID of thread being executed
        printf("Hello World from thread %d of %d\n", t_id, t_num);
    }

    return 0;
}
```
[Back to contents](#contents)
### Thread private variables
If we want the value of some variable to be persistent for a thread, we can define it as a thread private variable. This will ensure that the value will be attached to that thread regardless of which parallel block that thread is called in. `ThreadPrivateVar.c` provides a simple demonstration
```
int t_id;
#pragma omp threadprivate(t_id)
int main()
{
    int t_num;
#pragma omp parallel // Specify the block to be executed in parallel
    {
        t_id = omp_get_thread_num(); // Get ID of thread being executed
        if (t_id == 0)
            t_num = omp_get_num_threads(); // Get total number of OMP threads
    }

#pragma omp parallel // Specify the block to be executed in parallel
    printf("Hello World from thread %d of %d\n", t_id, t_num);
```
### #pragma omp barrier
So far, we've been using two different parallel regions to synchronize the threads an act as an implicit barrier. We can implement this behaviour more explicitly by using the directive `#pragma omp barrier`. This produces the same result as above, without the need to define an extra parallel region.
`OMPBarrier.c` implements `#pragma omp barrier`.
```
#include <omp.h> // OpenMP Header file
#include <stdio.h>

int main()
{
    int t_num;
#pragma omp parallel // Specify the block to be executed in parallel
    {
        int t_id = omp_get_thread_num(); // Get ID of thread being executed
        if (t_id == 0)
            t_num = omp_get_num_threads(); // Get total number of OMP threads

#pragma omp barrier
        printf("Hello World from thread %d of %d\n", t_id, t_num);
    }
    return 0;
}
```
### #pragma omp single
We've been looking at barriers so far to achieve one goal - make a single thread perform some action and then synchronize all threads before moving to the next parallel block. This can be done more easily with the compiler directive `#pragma omp single`, as shown in `OMPSingle.c`. Whichever thread comes across `#pragma omp single` first will execute it.
```
#include <omp.h> // OpenMP Header file
#include <stdio.h>

int main()
{
    int t_num;
#pragma omp parallel // Specify the block to be executed in parallel
    {
#pragma omp single
        t_num = omp_get_num_threads(); // Get total number of OMP threads

        int t_id = omp_get_thread_num(); // Get ID of thread being executed
        printf("Hello World from thread %d of %d\n", t_id, t_num);
    }
    return 0;
}
```
An implied barrier exists with `#pragma omp single` unless the `nowait` clause is applied to it
```
#pragma omp sinlge nowait
```
_This may cause an error depending on the application_
### #pragma omp master
If we want only the master thread to execute some code, we can use `#pragma omp master` instead of `#pragma omp single`. An implicit barrier does not exist, so this mathces `#pragma omp single nowait` in behaviour.
```
#include <omp.h> // OpenMP Header file
#include <stdio.h>

int main()
{
    int t_num;
#pragma omp parallel // Specify the block to be executed in parallel
    {
        int t_id;
#pragma omp master
        t_num = omp_get_num_threads(); // Get total number of OMP threads
        t_id = omp_get_thread_num(); // Get ID of thread being executed
        printf("Hello World from thread %d of %d\n", t_id, t_num);
    }
    return 0;
}
```

[Back to contents](#contents)