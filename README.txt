README

1.	What does this program do

This program checks for deadlocks in n worker threads with m resources. 
It takes number of threads, number of resources, deadlock checking interval d, total number of resources, time of execution as input. 
Then at every interval of d seconds, it checks to determine if a deadlock exists or not and then uses suitable heuristic to remove the deadlock.

2.	A description of how this program works (i.e. its logic)

First, this program takes various inputs from the user like: 
number of resources and total number of each of the resources,
deadlock checking interval d,
time of execution,
deadlock removal heuristic number and
number of threads.
Then each thread is assigned a request matrix, that is the total number of resources requested by the thread. 
Then the program spawns n worker threads, which makes resource requests of a particular type one at a time. 
Mutex lock is used so that only one thread can make a request at a particular time.
Another worker thraed is created, called deadlock thread, which is responsible for making deadlock checks at particular intervals. 
If a deadlock is present, then the deadlock thread uses suitable heuristic to remove the deadlock. 
Different heuristics implemented are:
preempt all deadlocked threads,
preempt threads in order of thread_id, till deadlock is not resolved,
preempt threads in order of maximum remaining requests, till deadlock is not resolved,
preempt threads in order of maximum allocated resources and
preempt thraeds in order of minimum allocated resources.
Finally deadlock threads prints deadlock statistics for the particular heuristic, that is, the time interval between deadlocks.
Deadlock thread is also enclosed in mutex locks, so that while deadlock thread is executing, no resource requests happen.
The program runs till 5*time of execution, using different heuristics for different 1/5 part of the time of execution.
Finally it outputs, time between deadlocks for different heuristics and the longest and shortest time achieving heuristic. 
So, if time of execution entered is 20, then the program will run for 100 seconds.
If input heuristic number is 3, then
0-20 seconds    ====> Heuristic 3 is used
20-40 seconds   ====> Heuristic 4 is used
40-60 seconds   ====> Heuristic 5 is used
60-80 seconds   ====> Heuristic 1 is used
80-100 seconds  ====> Heuristic 2 is used 

3.	How to compile and run this program

Following commands are written in the terminal:
gcc deadlock.c -o a -lpthread
./a

constraints:
deadlock interval >= 10 (for better results)
time of execution % deadlock interval = 0 (for correct heuristic result)

4.	Provide a snapshot of a sample run

Sample execution is shown below:

Enter number of resources: 3
Enter number of resources of type A : 10
Enter number of resources of type B : 20
Enter number of resources of type C : 30
Enter deadlock check interval: 10
Enter deadlock removal heuristic number: 3
Enter time of execution: 20
Enter number of threads: 3
Thread  1       has requested:  A: 4    B: 7    C: 28
Thread  2       has requested:  A: 6    B: 14   C: 26
Thread  3       has requested:  A: 3    B: 10   C: 2
Time:  4        Thread  1       has requested   12      of resource     C       Request Accepted
Time:  4        Thread  3       has requested   1       of resource     B       Request Accepted
Time:  5        Thread  2       has requested   3       of resource     B       Request Accepted
Time:  7        Thread  3       has requested   2       of resource     C       Request Accepted
Time:  8        Thread  2       has requested   19      of resource     C       Request Denied
Time:  9        Thread  1       has requested   2       of resource     B       Request Accepted
***********
Deadlock logic started
************
Allocation matrix
0       2       12
0       3       0
0       1       2

Request matrix
4       5       16
6       11      26
3       9       0

Available array: 10     14      16


Heuristic 3 is used
There is no deadlock

Allocation matrix
0       2       12
0       3       0
0       1       2

Request matrix
4       5       16
6       11      26
3       9       0

Available array: 10     14      16

*************
Deadlock statistics
Number of deadlocks: 0 and Time: 10
Time per deadlock: inf
**************
Time: 12        Thread  1       has requested   11      of resource     C       Request Accepted
Time: 12        Thread  2       has requested   6       of resource     A       Request Accepted
Time: 13        Thread  3       has requested   8       of resource     B       Request Accepted
Time: 16        Thread  3       has requested   1       of resource     B       Request Accepted
Time: 16        Thread  1       has requested   4       of resource     A       Request Accepted
Time: 16        Thread  2       has requested   5       of resource     B       Request Accepted
***********
Deadlock logic started
************
Allocation matrix
4       2       23
6       8       0
0       10      2

Request matrix
0       5       5
0       6       26
3       0       0

Available array: 0      0       5

Thread 1 is in deadlock
Thread 2 is in deadlock
Thread 3 is in deadlock

Heuristic 3 is used
Thread 2 is preempted

Allocation matrix
4       2       23
0       0       0
0       10      2

Request matrix
0       5       5
6       14      26
3       0       0

Available array: 6      8       5


Heuristic 3 is used
There is no deadlock

Allocation matrix
4       2       23
0       0       0
0       10      2

Request matrix
0       5       5
6       14      26
3       0       0

Available array: 6      8       5

*************
Deadlock statistics
Number of deadlocks: 1 and Time: 20
Time per deadlock: 20.000000
**************
Time: 22        Thread  1       has requested   2       of resource     C       Request Accepted
Time: 22        Thread  2       has requested   3       of resource     C       Request Accepted
Time: 25        Thread  1       has requested   2       of resource     B       Request Accepted
Time: 26        Thread  3       has requested   3       of resource     A       Request Accepted
Time: 27        Thread  2       has requested   14      of resource     C       Request Denied
***********
Deadlock logic started
************
Allocation matrix
4       4       25
0       0       3
3       10      2

Request matrix
0       3       3
6       14      23
0       0       0

Available array: 3      6       0

Thread 1 is in deadlock
Thread 2 is in deadlock

Heuristic 4 is used
Thread 1 is preempted

Allocation matrix
0       0       0
0       0       3
3       10      2

Request matrix
4       7       28
6       14      23
0       0       0

Available array: 7      10      25


Heuristic 4 is used
There is no deadlock

Allocation matrix
0       0       0
0       0       3
3       10      2

Request matrix
4       7       28
6       14      23
0       0       0

Available array: 7      10      25

*************
Deadlock statistics
Number of deadlocks: 1 and Time: 10
Time per deadlock: 10.000000
**************
Time: 32        Thread  2       has requested   16      of resource     C       Request Accepted
Time: 32        Thread  1       has requested   2       of resource     A       Request Accepted
Thread 3 will terminate as it has acquired all resources
Thread 3 has requested: A : 7 B : 14 C : 14 
Time: 34        Thread  2       has requested   1       of resource     C       Request Accepted
Time: 38        Thread  3       has requested   1       of resource     A       Request Accepted
Time: 38        Thread  1       has requested   2       of resource     A       Request Accepted
Time: 39        Thread  2       has requested   2       of resource     C       Request Accepted
***********
Deadlock logic started
************
Allocation matrix
4       0       0
0       0       22
1       0       0

Request matrix
0       7       28
6       14      4
6       14      14

Available array: 5      20      8

Thread 1 is in deadlock
Thread 2 is in deadlock
Thread 3 is in deadlock

Heuristic 4 is used
Thread 2 is preempted

Allocation matrix
4       0       0
0       0       0
1       0       0

Request matrix
0       7       28
6       14      26
6       14      14

Available array: 5      20      30


Heuristic 4 is used
There is no deadlock

Allocation matrix
4       0       0
0       0       0
1       0       0

Request matrix
0       7       28
6       14      26
6       14      14

Available array: 5      20      30

*************
Deadlock statistics
Number of deadlocks: 2 and Time: 20
Time per deadlock: 10.000000
**************
Time: 43        Thread  2       has requested   2       of resource     A       Request Accepted
Time: 43        Thread  3       has requested   7       of resource     B       Request Accepted
Time: 45        Thread  1       has requested   12      of resource     C       Request Accepted
Time: 47        Thread  1       has requested   11      of resource     C       Request Accepted
Time: 47        Thread  2       has requested   2       of resource     A       Request Accepted
Time: 49        Thread  3       has requested   1       of resource     A       Request Accepted
***********
Deadlock logic started
************
Allocation matrix
4       0       23
4       0       0
2       7       0

Request matrix
0       7       5
2       14      26
5       7       14

Available array: 0      13      7

Thread 2 is in deadlock
Thread 3 is in deadlock

Heuristic 5 is used
Thread 2 is preempted

Allocation matrix
4       0       23
0       0       0
2       7       0

Request matrix
0       7       5
6       14      26
5       7       14

Available array: 4      13      7


Heuristic 5 is used
There is no deadlock

Allocation matrix
4       0       23
0       0       0
2       7       0

Request matrix
0       7       5
6       14      26
5       7       14

Available array: 4      13      7

*************
Deadlock statistics
Number of deadlocks: 1 and Time: 10
Time per deadlock: 10.000000
**************
Time: 52        Thread  1       has requested   2       of resource     C       Request Accepted
Time: 52        Thread  2       has requested   20      of resource     C       Request Denied
Time: 53        Thread  3       has requested   7       of resource     C       Request Denied
Time: 54        Thread  1       has requested   4       of resource     B       Request Accepted
Time: 54        Thread  2       has requested   4       of resource     B       Request Accepted
Time: 57        Thread  2       has requested   2       of resource     A       Request Accepted
Time: 59        Thread  1       has requested   3       of resource     B       Request Accepted
Time: 59        Thread  3       has requested   1       of resource     A       Request Accepted
***********
Deadlock logic started
************
Allocation matrix
4       7       25
2       4       0
3       7       0

Request matrix
0       0       3
4       10      26
4       7       14

Available array: 1      2       5


Heuristic 5 is used
There is no deadlock

Allocation matrix
4       7       25
2       4       0
3       7       0

Request matrix
0       0       3
4       10      26
4       7       14

Available array: 1      2       5

*************
Deadlock statistics
Number of deadlocks: 1 and Time: 20
Time per deadlock: 20.000000
**************
Time: 62        Thread  2       has requested   10      of resource     C       Request Denied
Time: 62        Thread  3       has requested   4       of resource     B       Request Denied
Time: 64        Thread  1       has requested   1       of resource     C       Request Accepted
Time: 66        Thread  3       has requested   1       of resource     A       Request Accepted
Time: 68        Thread  2       has requested   19      of resource     C       Request Denied
***********
Deadlock logic started
************
Allocation matrix
4       7       26
2       4       0
4       7       0

Request matrix
0       0       2
4       10      26
3       7       14

Available array: 0      2       4


Heuristic 1 is used
There is no deadlock

Allocation matrix
4       7       26
2       4       0
4       7       0

Request matrix
0       0       2
4       10      26
3       7       14

Available array: 0      2       4

*************
Deadlock statistics
Number of deadlocks: 0 and Time: 10
Time per deadlock: inf
**************
Time: 72        Thread  3       has requested   6       of resource     C       Request Denied
Time: 74        Thread  2       has requested   2       of resource     A       Request Denied
Time: 75        Thread  3       has requested   9       of resource     C       Request Denied
Time: 76        Thread  2       has requested   24      of resource     C       Request Denied
Time: 78        Thread  2       has requested   1       of resource     C       Request Accepted
***********
Deadlock logic started
************
Allocation matrix
4       7       26
2       4       1
4       7       0

Request matrix
0       0       2
4       10      25
3       7       14

Available array: 0      2       3


Heuristic 1 is used
There is no deadlock

Allocation matrix
4       7       26
2       4       1
4       7       0

Request matrix
0       0       2
4       10      25
3       7       14

Available array: 0      2       3

*************
Deadlock statistics
Number of deadlocks: 0 and Time: 20
Time per deadlock: inf
**************
Time: 82        Thread  3       has requested   8       of resource     C       Request Denied
Time: 83        Thread  1       has requested   2       of resource     C       Request Accepted
Time: 84        Thread  2       has requested   3       of resource     B       Request Denied
Time: 85        Thread  3       has requested   7       of resource     C       Request Denied
Time: 87        Thread  3       has requested   13      of resource     C       Request Denied
Time: 87        Thread  2       has requested   21      of resource     C       Request Denied
***********
Deadlock logic started
************
Allocation matrix
4       7       28
2       4       1
4       7       0

Request matrix
0       0       0
4       10      25
3       7       14

Available array: 0      2       1


Heuristic 2 is used
There is no deadlock

Allocation matrix
4       7       28
2       4       1
4       7       0

Request matrix
0       0       0
4       10      25
3       7       14

Available array: 0      2       1

*************
Deadlock statistics
Number of deadlocks: 0 and Time: 10
Time per deadlock: inf
**************
Time: 92        Thread  3       has requested   2       of resource     B       Request Accepted
Thread 1 will terminate as it has acquired all resources
Thread 1 has requested: A : 8 B : 8 C : 3 
Time: 93        Thread  2       has requested   2       of resource     A       Request Accepted
Time: 95        Thread  1       has requested   4       of resource     B       Request Accepted
Time: 96        Thread  2       has requested   14      of resource     C       Request Accepted
Time: 97        Thread  3       has requested   5       of resource     B       Request Denied
Time: 99        Thread  2       has requested   1       of resource     A       Request Accepted
***********
Deadlock logic started
************
Allocation matrix
0       4       0
5       4       15
4       9       0

Request matrix
8       4       3
1       10      11
3       5       14

Available array: 1      3       15

Thread 1 is in deadlock
Thread 2 is in deadlock
Thread 3 is in deadlock

Heuristic 2 is used
Thread 1 is preempted

Allocation matrix
0       0       0
5       4       15
4       9       0

Request matrix
8       8       3
1       10      11
3       5       14

Available array: 1      7       15

Thread 2 is in deadlock
Thread 3 is in deadlock

Heuristic 2 is used
Thread 2 is preempted

Allocation matrix
0       0       0
0       0       0
4       9       0

Request matrix
8       8       3
6       14      26
3       5       14

Available array: 6      11      30


Heuristic 2 is used
There is no deadlock

Allocation matrix
0       0       0
0       0       0
4       9       0

Request matrix
8       8       3
6       14      26
3       5       14

Available array: 6      11      30

*************
Deadlock statistics
Number of deadlocks: 1 and Time: 20
Time per deadlock: 20.000000
**************
Heuristic 1: inf
Heuristic 2: 20.000000
Heuristic 3: 20.000000
Heuristic 4: 10.000000
Heuristic 5: 20.000000
Longest average time between deadlocks: 1
Shortest average time between deadlocks: 4
