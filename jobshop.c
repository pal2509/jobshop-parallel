#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define MAX 100

int maxJobs = 0;
int maxMan = 0;

// Matriz para o tempo de cada job em cada maquina
int jobTime[MAX][MAX];

// Matriz para o numero de cada maquina de cada operação
int jobM[MAX][MAX];

// Matriz para o tempo de inicio de cada operação ---> Solução
int jobSolution[MAX][MAX];

void PrintJobs(int y, int x, int jobSolution[y][x], int totalTime);
void ReadFile(char *fileName);

int main(int argc, char *argv[])
{

    FILE *fpOutput;
    char *fileNameI;
    char *fileNameO;

    if (argc < 1)
        printf("Insira argumentos.\n");
    else
    {
        printf("%s\n", argv[1]);
        printf("%s\n", argv[2]);

        fileNameI = (char *)malloc(strlen(argv[1]) + 1);
        strcpy(fileNameI, argv[1]);
        fileNameO = (char *)malloc(strlen(argv[1]) + 1);
        strcpy(fileNameO, argv[1]);

        ReadFile(fileNameI);
    }

    int min = 0;

    // Contador dos instantes de tempo
    int time = 0;

    // // Calculo dos tempos de inicio
    // for (int y = 0; y < 3; y++)
    // {
    //     for (int x = 0; x < 3; x++)
    //     {
    //         jobSolution[y][x] = time;
    //         time += jobTime[y][x];
    //     }
    // }

    // // Escrita do resultado na consola
    // PrintJobs(maxMan, maxJobs, jobSolution, time);

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

void ReadFile(char *fileName)
{

    FILE *fpInput = fopen(fileName, "r");
    if (!fpInput)
    {
        perror("fopen");
        return;
    }

    // fread(buffer, sizeof(*buffer), ARRAY_SIZE(buffer), fpInput);

    char buffer[2048];

    int x = 0, y = 0;

    fscanf(fpInput, "%d %d", &maxJobs, &maxMan);
    printf("Matrix size:\nNr lines:%d\nNr colums:%d\n", maxJobs, maxMan);

    fread(buffer, sizeof(*buffer), ARRAY_SIZE(buffer), fpInput);

    char *delim = " ";

    char *ptr = strtok(buffer, delim);

    int i = 0, nr = 0, job = 0, time = 0, man = 0;

    while (ptr != NULL)
    {
        nr = atoi(ptr);
        // printf("%d ", nr);

        if (i == maxMan * 2 - 1)
        {

            man = 0;
            time = 0;
            i = 0;
            job++;
        }

        if (i % 2 == 0)
        {

            jobM[job][man] = nr;
            man++;
        }
        else
        {

            jobTime[job][time] = nr;
            time++;
        }

        ptr = strtok(NULL, delim);
        i++;
    }

    // printf("\n");
    // printf("%d %d\n", maxJobs, maxMan);

    // for (int i = 0; i < maxJobs; i++)
    // {
    //     printf("job nr:%d\n", i);
    //     for (int j = 0; j < maxMan; j++)
    //     {
    //         printf("M:%d ", jobM[i][j]);
    //         printf("T:%d \n", jobTime[i][j]);
    //     }
    // }

    fclose(fpInput);
}