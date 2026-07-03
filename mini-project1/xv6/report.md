# xv6 Modifications Report
## Part A: getreadcount System Call
We implemented the `sys_getreadcount` system call to track the total number of bytes read across all processes since boot. A global counter `readcount` and a spinlock `readcount_lock` were added to `kernel/sysfile.c`. Each time `sys_read` is called and successfully reads bytes, it increments the `readcount`. The system call retrieves this value safely.

## Part B: Schedulers

### FCFS (First Come First Serve)
We modified `kernel/proc.h` to store the creation time `ctime` of each process (initialized in `allocproc`). In the scheduler loop, instead of picking the first runnable process, we iterate through the process table to find the one with the lowest `ctime`. We also disabled timer interrupts from yielding control.

### CFS (Completely Fair Scheduler)
We added the `nice` and `vruntime` parameters. `vruntime` acts as a normalized representation of how long a process has run based on its nice value. The scheduler now iterates over all processes, selecting the one with the smallest `vruntime`. When the process runs, its `vruntime` increases, pushing it down the priority list.

### MLFQ (Preemptive Multi-Level Feedback Queue)
We added a `queue` and `ticks_used` field. All new processes start in queue 0. The scheduler always chooses runnable processes in the highest-priority non-empty queue (0 to 3). During timer interrupts, `ticks_used` is incremented; when a process exhausts its time slice (1, 4, 8, 16 ticks), it drops to a lower queue. 

### Performance Comparison
Round Robin provides high responsiveness but doesn't differentiate process needs.
FCFS gives great throughput for batch jobs but poor responsiveness.
CFS balances interactivity and throughput seamlessly based on priority (nice).
MLFQ approximates SJF, optimizing turnaround time by adjusting priorities dynamically.

