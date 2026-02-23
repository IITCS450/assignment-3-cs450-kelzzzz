# Lottery Scheduler Experiment Results

## 1. Setup
The experiment was conducted on xv6 using a lottery scheduling algorithm. There is a DEBUG flag set to `1` in proc.c to print tickets given to processes.
- **Environment:** xv6 on QEMU.
- **Process Count:** 3 child processes per test.
- **Ticket Distribution:** 
    - Test A (Short/Long): 20, 40, 80 tickets (Ratio 1:2:4)
    - Test B (Similar): 2, 4, 6 tickets (Ratio 1:2:3)

## 2. Workload
The workload consists of a CPU-bound loop within `testlottery.c`, which runs `burn()` an `n` amount of times. The short test loops around `10000000` times and the long `200050000`. `burn()` increments a counter.

Each child process executes the task to ensure they remain in the `RUNNABLE` state, demanding maximum CPU time from the scheduler.

## 3. Observed Relative Shares
Based on the results, there is a clear distinction between short-burst workloads and sustained workloads.

### Short Runs vs. Long Runs


- **Short Runs:** In the short tests, the exit order was inconsistent. For example, in Trial 1, the child with 20 tickets exited before the child with 80 tickets, and in another the child with 40 exited before 20 and 80.

- **Long Runs:** In the long tests, the scheduler consistently favored the higher ticket counts. In all three trials, the exit order was 80 -> 40 -> 20.

### Convergence and Variance
The experimental data is evidence of the probabilistic nature of a lottery scheduler:
1. **Variance:** In the short term, RNG plays a significant role. A process with fewer tickets can win several consecutive time slices because there are fewer "draws" of tickets overall. Additionally, as ticket counts are smaller, variance is a little greater since they have closer to equal chances of winning.
2. **Convergence:** As the number of scheduling events ($n$) increases, the CPU time allocated to each process approaches the probability ($tickets / total\_tickets$). This is why the long tests more accurately reflect the ticket ratios, while short tests do not.

## 4. Conclusion
The implementation successfully validates that ticket counts influence CPU share. While high variance is present in small workloads, the scheduler achieves fair proportional sharing over longer execution periods.

### Result Screenshots
![Run 1](run1.png)
![Run 2](run2.png)
![Run 3](run3.png)
