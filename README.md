# Job-shop scheduling in C: sequential and parallel (OpenMP)

A course project for high-performance computing (CompAD) in my Master's in Software
Engineering at IPCA, written between March and May 2022. It schedules the classic
**job-shop problem** with a greedy heuristic, then parallelises the scheduler with OpenMP and
measures what that gains, and what it costs.

## The problem

There are *n* jobs and *m* machines. Each job is a fixed sequence of operations, and each
operation needs one specific machine for a given time. A machine runs one operation at a time,
and a job's operations run in order. The goal is a schedule that finishes everything as early
as possible: the smallest **makespan**.

## The approach

- **`jobshop.c`, sequential.** The scheduler works in rounds over a shared machine timeline.
  In each round it walks the jobs and places every operation whose previous operation is done
  and whose machine is free at the current time, then moves the clock forward by the longest
  operation it placed. It repeats until every operation has a start time, then checks the
  schedule is valid (order kept, no machine double-booked).
- **`jobshop_paralelo.c`, parallel.** The jobs are split into equal blocks, one per OpenMP
  thread. Each thread schedules its own block on its own clock, writing into the same
  machine timeline; the insert into that timeline is an `omp critical` section, so two
  threads never book the same slot.

## Build and run

```bash
gcc -O2 -fopenmp -o jobshop jobshop.c
gcc -O2 -fopenmp -o jobshop_parallel jobshop_paralelo.c -lpthread

./jobshop ft/ft10.txt out.txt               # input, output
./jobshop_parallel 4 ft/ft10.txt out.txt    # threads, input, output
```

The program's messages are in Portuguese: it prints the makespan ("Tempo total…"), whether
the schedule checks out ("Verificação da solução: Correta") and the run time. The output
file holds the matrix size, then the start time of each operation, one job per line.

### Input format

`ft/` holds the instances. The first line is `jobs machines`; then one line per job with
`machine duration` pairs, in the order the job's operations must run. `ft06`, `ft10` and
`ft20` are the Fisher & Thompson benchmarks; `ta80` is a 100 × 20 Taillard instance; `ft03`,
`ft15`, `ft2020` and `ft5015` are sizes used in the course.

## Results

Measured on 2026-11-06 with GCC 14 on a 6-core / 12-thread AMD Ryzen 5 2600X.

**Schedule quality, sequential version** (makespan, lower is better):

| Instance | Size | This scheduler | Best known | Ratio |
|---|---|---:|---:|---:|
| ft06 | 6 × 6 | 129 | 55 | 2.3× |
| ft10 | 10 × 10 | 3,447 | 930 | 3.7× |
| ft20 | 20 × 5 | 3,096 | 1,165 | 2.7× |

**Parallel version, by thread count:**

| Threads | ft10 makespan | ft10 run time | ta80 makespan | ta80 run time |
|---:|---:|---:|---:|---:|
| 1 | 2,211 | 0.04 ms | 23,456 | 0.6 ms |
| 2 | 1,856 | 0.6 ms | 18,928 | 1.2 ms |
| 4 | 1,469 | 0.7 ms | 16,387 | 2.4 ms |
| 8 | 1,609 | 0.5 ms | 8,602 | 3.9 ms |
| 12 | 2,211 | 0.5 ms | 10,511 | 7.8 ms |

For comparison, the sequential version gives 3,447 on ft10 and 24,447 on ta80. The 2022
measurements in `speedup.xlsx` tell the same story: a speed-up of 0.89 with 2 threads, falling
to 0.22 with 12.

## What the numbers say

- **Parallel made it slower, not faster.** A whole schedule takes well under a millisecond,
  so starting threads costs more than the work they share, and the critical section around
  every insert makes them queue for the one shared timeline.
- **Parallel changed the answer.** Because each thread runs its own clock over its own block
  of jobs, the threads interleave operations differently, which here often gives a shorter
  schedule. That's a side effect of the split, not a better algorithm by design.
- **The greedy rule is far from optimal:** 2.3 to 3.7 times the best known makespan.

## What I'd change today

- Time the sequential version correctly: `clock() / CLOCKS_PER_SEC` is integer division, so
  it rounds every run down to 0 seconds.
- Replace the 100,000 × 1,000 machine timeline (about 400 MB, reset ten times per run) with
  a per-machine list of busy intervals.
- Use a real heuristic (dispatching rules, then tabu search or branch and bound) and
  parallelise the *search*, where there is enough work to share, rather than one greedy pass.

`speedup.xlsx` holds the 2022 measurements.

## Licence

MIT, see [LICENSE](LICENSE).
