#include <stdio.h>

void PrintJobs(int y, int x, int jobSolution[y][x], int totalTime);

int main(int argc, char *argv[])
{

    int min = 0;

    int jobTime[3][3] = {
        {3, 2, 2},
        {2, 1, 4},
        {4, 3, 1},
    };

    int jobM[3][3] = {
        {0, 3, 2},
        {0, 2, 1},
        {1, 2, 3},
    };

    int jobSolution[3][3];

    int time = 0;

    for (int y = 0; y < 3; y++)
    {
        for (int x = 0; x < 3; x++)
        {
            jobSolution[y][x] = time;
            time += jobTime[y][x];
        }
    }

    PrintJobs(3, 3, jobSolution, time);

    return 1;
}

// job0 (m0,3) m(1,2) m(2,2)
// job1 m(0,2) m(2,1) m(1,4)
// job2 m(1,4) m(2,3)  -----

// Escreve a job solution
//  Job 0: m0 t3	m1 t2	m2 t2
//  Job 1: m0 t2	m2 t1	m1 t4
//  Job 2: m1 t4	m2 t3	m-1 t-1
void PrintJobs(int y, int x, int jobSolution[y][x], int totalTime)
{
    printf("\t\t");
    for (int i = 0; i < x; i++)
    {
        printf("O-%d\t\t", i);
    }

    printf("\n");

    for (int i = 0; i < y; i++)
    {
        printf("Job %d: ", i);

        for (int j = 0; j < x; j++)
            printf("Start time: %d\t", jobSolution[i][j]);
        printf("\n");
    }
    printf("Total time %d: \n", totalTime);
}

// Pior solução
// Meter em matriz Job x Operação
// 3 arrays, um com duração, um com a maquina, e um de output com o instante de tempo de inicio
// Atribuir sequencialmente na diagonal do array o inicio de cada operação
//  Job 0: m0 t3	m1 t2	m2 t2
//  Job 1: m0 t2	m2 t1	m1 t4
//  Job 2: m1 t4	m2 t3	m-1 t-1
//
//          O1  O2  O3
//  Job 1   0   3   5
//  Job 2   7   9   10
//  Job 3   14  18  21 -->22
//
// Contador tempo
//
// Heuristica local
// Local heuristic neighborhood search
//
// Cada thread fazer a atribuição de cada job
//
// Matriz de ocupação da maquina para controlar o acesso á maquina
// Ou variável global para cada máquina
//
// Solução otima matemática
// Branch and bound para jobshop
// Cresce exponencialmente
//
//
//