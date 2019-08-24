Solving the Readers-Writers and Fair Readers-Writers Problem using Semaphores.
Comparing the average time and worst case time taken for each thread to enter CS.(for both the algorithms).

Input: The input to the programs will be a file, named inp-params.txt, consisting of the following parameters:
nw: the number of writer threads, nr: the number of reader threads,
kw: the number of times each writer thread tries to enter the CS,
kr: the number of times each reader thread tries enter the CS,

μCS, μRem: t1 and t2 are delay values that are exponentially distributed with an average of μCS, μRem seconds.
The objective of these time delays is to simulate that these threads are performing some time consuming tasks.

Output: For both algorithms, following two files are generated:
1.  A log of events demonstrating that readers and writers are accessing the CS mutually exclusively but multiple readers are allowed at the same time. 
2.  "Average_time.txt" containing average time taken by a thread to enter CS.

Report: Explanation about design of the program and a comparision between Readers and Writers threads.
For more information about the metrics of comparision, check out **ReportAssgn4-CS17BTECH11011.pdf**.

To know about how to execute the programs, read **ReadmeAssgn4-CS17BTECH11011.txt**.
