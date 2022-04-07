#include <stdio.h>

void PrintJobs(int jobCount, int mCount, int jobs[jobCount][mCount]);

int main(int argc, char *argv[])
{
    int jobs[3][6] = {
        {0, 3, 1, 2, 2, 2},
        {0, 2, 2, 1, 1, 4},
        {1, 4, 2, 3, -1, -1}};

    int min = 0;

    PrintJobs(3, 6, jobs);

    return 1;
}

// Meter os jobs em arrays

// job0 (m0,3) m(1,2) m(2,2)
// job1 m(0,2) m(2,1) m(1,4)
// job2 m(1,4) m(2,3)  -

// Escreve os jobs na consola
//  Job 0: m0 t3	m1 t2	m2 t2
//  Job 1: m0 t2	m2 t1	m1 t4
//  Job 2: m1 t4	m2 t3	m-1 t-1
void PrintJobs(int jobCount, int mCount, int jobs[jobCount][mCount])
{
    for (int i = 0; i < jobCount; i++)
    {
        printf("Job %d: ", i);

        for (int m = 0; m < mCount; m = m + 2)
            printf("m%d t%d\t", jobs[i][m], jobs[i][m + 1]);
        printf("\n");
    }
}
