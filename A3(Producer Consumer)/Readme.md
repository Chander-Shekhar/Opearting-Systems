Solving the bounded buffer Producer-Consumer Problem using Semaphores and Locks.
Comparing the average time taken by producer and consumer threads (for both using semaphores and locks).

Input: The input to the programs will be a file, named inp-params.txt, consisting of the following parameters:
capacity, np, nc, cntp, cntc, μp, μc.
capacity: Size of Buffer, np: No. of Producer threads, nc: No. of consumer threads,
cntp: No. of times each producer thread will produce an item,
cntc: No. of times each consumer thread will consume an item,
μp, μc: t1 and t2 are delay values that are exponentially distributed with an average of μp, μc seconds.
The objective of these time delays is to simulate that these threads are performing some time consuming tasks.

Output: A log of events demonstrating following properties:
1.  A producer thread produces items only when there is sufficient space left in the buffer.
2.  A consumer thread consumes only those items that are indeed produced by a producer thread.

Report: Explanation about design of the program and a comparision between producer and consumer threads.

To know about how to execute the programs, read ReadmeAssgn3-CS17BTECH11011.txt
