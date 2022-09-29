# A (gentle) introduction to OpenMP
# Preface
This repo has been created for students of DS-226 _Introduction to Computing for AI & ML_. The codes within the `OpenMP` folder aim to illustrate some principles of parallel programming on shared-memory systems using the OpenMP paradigm. 
You can download the repo as a `.zip` file from GitHub or clone it
```
git clone https://github.com/divijghose/DS226-ICAIML
```
Then go to the `OpenMP` folder
```
cd Codes/OpenMP
```
The `makefile` will compile all the programs and generate their output files with the same name
```
make
```



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
[Parallelization using OpenMP](#parallelization-using-openmp)
6. [Timing your code](#timing-your-code)
7. [Computing the sum of an array](#computing-the-sum-of-an-array)\
    a. [The wrong way](#the-wrong-way)\
    b. [The wrong way, again - critical sections](#the-wrong-way-again---critical-sections)\
    c. [The correct way - #pragma omp for](#the-correct-way---pragma-omp-for)
8. [Going ahead](#going-ahead)

## What is OpenMP? 
OpenMP (Open Multi-Processing) is an API that provides support for parallel programming in **shared-memory architectures**. It consists of compiler directives which are inserted in the code in regions that can be parallelised, a run-time library to execute these regions in parallel, and environment variables. OpenMP supports parallelism in C, C++ and Fortran, and is preffered for its portability and ease of use.
OpenMP uses a _thread-based, fork-join model_.

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
# Parallelization using OpenMP 
## Timing your code
`omp_get_wtime` and `omp_get_wtick` can be used to to get time in seconds (starting from some universal time) and the resolution of the timer respectively.

[Back to contents](#contents)
## Computing the sum of an array
### Timing the sequential implementation
Using `omp_get_wtime` to get the time stamps before and after the sequential execution of the sum of an array, we can get a rough idea of the benchmark for our parallel codes. Compile and execute `SumArraySequential.c` with an appropriate value of `ARRAY_SIZE` and check for yourself.
```
#include <omp.h>
#include <stdio.h>

#define ARRAY_SIZE 10000
int arr[ARRAY_SIZE];

int main()
{
    int i, sum_arr = 0;
    double t_start, t_end;

    for (i = 0; i < ARRAY_SIZE; i++) // Initialize array
        arr[i] = 1;

    t_start = omp_get_wtime();
    for (i = 0; i < ARRAY_SIZE; i++) // Sum up the array
        sum_arr += arr[i];
    t_end = omp_get_wtime();

    printf("Sum of array elements = %d. Time required to execute = %f seconds\n", sum_arr, t_end - t_start);

    return 0;
}
```
### The wrong way
Consider the introduction of a parallel region in `SumArrayParallel_RacePlusRedundant.c`
```
    t_start = omp_get_wtime();
#pragma omp parallel
    {
        for (i = 0; i < ARRAY_SIZE; i++) // Sum up the array
            sum_arr += arr[i];
    }
    t_end = omp_get_wtime();

    printf("Sum of array elements = %d. Time required to execute = %f seconds\n", sum_arr, t_end - t_start);

```
Clearly, there's a race condition on `sum_arr`. But this also has another problem - each thread tries to calculate the sum of the array independently, thereby making the whole operation redundant. Since each thread has its own cache overheads, the time required to execute this code is also more than the sequential one.

### The wrong way, again - critical sections
To remove the redundancy in the previous code, we can make each thread compute part of the sum by manually distributing the array between the threads, and define a critical section for the sum calculation. A critical section is one which can only be accesed by one thread at a time, thereby eliminating race conditions. 
```
#pragma omp parallel default(shared) private(i, t_id)
    {
        int arr_start, arr_end;
        t_id = omp_get_thread_num();
        t_num = omp_get_num_threads();
        // Define the start and end points for the sub-array
        arr_start = t_id * (ARRAY_SIZE / t_num);
        arr_end = (t_id + 1)*(ARRAY_SIZE / t_num) - 1;
        if (t_id == t_num) // for the last subarray
            arr_end = ARRAY_SIZE - 1;
        // printf("Thread %d of %d starts at %d and ends at %d\n",t_id,t_num,arr_start,arr_end);

        for (i = arr_start; i <= arr_end; i++) // Sum up the array
            #pragma omp critical
            sum_arr += arr[i];
    }
```
Critical sections must be used catiously, though, as they have considerable overheads that slow down the code.

### The correct way - #pragma omp for
One correct way to distribute the work and use critical sections is to calculate partial sums of the contents of each thread (using the OpenMP for loop directive), and then add these partials sums into the total sums in a critical section.The corrected code can be found in `SumArrayParallel_Corrected`.
```
#include <omp.h>
#include <stdio.h>

#define ARRAY_SIZE 100000
int arr[ARRAY_SIZE];

int main()
{
    int i, t_id, t_num, sum_arr = 0;
    double t_start, t_end;

    for (i = 0; i < ARRAY_SIZE; i++) // Initialize array
        arr[i] = 1;

    t_start = omp_get_wtime();
#pragma omp parallel default(shared) private(i, t_id)
    {
        int psum = 0;
#pragma omp for
        for (i = 0; i <= ARRAY_SIZE; i++)
            psum += arr[i]; // Sum up the array

#pragma omp critical
        sum_arr += psum;
    }
    t_end = omp_get_wtime();

    printf("Sum of array elements = %d. Time required to execute = %f seconds\n", sum_arr, t_end - t_start);

    return 0;
}
```
There exists a more succint way of writing the for loop directive, and we can also get rid of the `psum` variable by applying a reduction on `sum_arr`. A cleaner version of the code above can be found in `SumArrayParallel.c`
 ```
 #include <omp.h>
#include <stdio.h>

#define ARRAY_SIZE 1000000000
int arr[ARRAY_SIZE];

int main()
{
    int i, sum_arr = 0;
    double t_start, t_end;

    for (i = 0; i < ARRAY_SIZE; i++) // Initialize array
        arr[i] = 1;

    t_start = omp_get_wtime();
#pragma omp parallel for default(shared) reduction(+: sum_arr)
    
        for (i = 0; i <= ARRAY_SIZE; i++)
            sum_arr += arr[i]; // Sum up the array
    
    t_end = omp_get_wtime();

    printf("Sum of array elements = %d. Time required to execute = %f seconds\n", sum_arr, t_end - t_start);

    return 0;
}
 ```

[Back to contents](#contents)

## Going Ahead
Once you're comfortable with the concepts listed above, you can try parallelizing some other linear algebra routines. In particular, you can start with - 
1. Vector-Vector dot product
2. Matrix-Vector multiplication
3. Matrix-Matrix multiplication

While working on these problems, you'll come across methods to further optimize your code (blocking, scheduling, etc.). Given below are some references to help you out:
1. [List of texts on OpenMP](https://www.openmp.org/resources/openmp-books/)
2. [Introduction to Parallel Computing by LLNL](https://hpc.llnl.gov/documentation/tutorials/introduction-parallel-computing-tutorial)
3. [OpenMP Tutorial by LLNL](https://hpc-tutorials.llnl.gov/openmp/)

[Back to contents](#contents)

