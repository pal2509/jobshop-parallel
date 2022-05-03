#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <omp.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define MAX 1000
#define MAXTIME 10000

int maxJobs = 0;
int maxMac = 0;

int thread_count;
long counter = 0;

pthread_mutex_t mutex;

// Matriz para o tempo de cada job em cada maquina
int jobTime[MAX][MAX];

// Matriz para o numero de cada maquina de cada operação
int jobM[MAX][MAX];

// Matriz para o tempo de inicio de cada operação ---> Solução
int jobSolution[MAX][MAX];

int Machines[MAXTIME][MAX];

void PrintJobs(int y, int x, int totalTime);
void ReadFile(char *fileName);
int InsertJob(int machine, int time, int jobDuration, int jobNr);
bool verifyPreviousOp(int job, int op);
int VerifySolution();
int JobShop();

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
        printf("%s\n", argv[3]);
        omp_set_num_threads(atoi(argv[1]));
        fileNameI = (char *)malloc(strlen(argv[2]) + 1);
        strcpy(fileNameI, argv[2]);
        fileNameO = (char *)malloc(strlen(argv[3]) + 1);
        strcpy(fileNameO, argv[3]);

        ReadFile(fileNameI);
    }

    // ALGORITMO SIMPLES PARA A PIOR SOLUÇÃO POSSIVEL
    //  printf("Jobs: %d\n", maxJobs);
    //  printf("Machines: %d\n", maxMac);
    //  // Calculo dos tempos de inicio
    //  for (int y = 0; y < maxJobs; y++)
    //  {
    //      for (int x = 0; x < maxMac; x++)
    //      {
    //          jobSolution[y][x] = time;
    //          time += jobTime[y][x];
    //      }
    //  }

    // Por os arrays com os valores a -1
    for (int i = 0; i < MAX; i++)
        for (int j = 0; j < MAX; j++)
        {
            jobSolution[i][j] = -1;
        }

    for (int i = 0; i < MAXTIME; i++)
        for (int j = 0; j < MAX; j++)
        {
            Machines[i][j] = -1;
        }

    // long thread;
    // pthread_t *thread_handles;

    // thread_count = strtol(argv[1], NULL, 10);

    // thread_handles = malloc(thread_count * sizeof(pthread_t));

    // printf("Thread_count: %d\n", thread_count);

    // for (thread = 0; thread < thread_count; thread++)
    // {
    //     pthread_create(&thread_handles[thread], NULL, JobShop, (void *)thread);
    // }
    // //  printf("Hello from the main thread\n");

    // for (thread = 0; thread < thread_count; thread++)
    //     pthread_join(thread_handles[thread], NULL);

    // printf("Main thread: total %ld\n", counter);

    // free(thread_handles);

    int time = JobShop();

    printf("\n\t\tSOLUÇÃO\n");
    // Escrita do resultado na consola
    PrintJobs(maxJobs, maxMac, time);
    printf("\nverify: %d\n", VerifySolution());
    return 1;
}

bool verifyPreviousOp(int job, int op)
{
    if (op > 0)
    {
        if (jobSolution[job][op - 1] == -1)
            return false;
        else
            return true;
    }
    else
        return true;
}

int InsertJob(int machine, int time, int jobDuration, int jobNr)
{
    // printf("insert:%d %d ", machine, jobNr);

    int slots = 0;
    for (int i = time; i < time + jobDuration; i++)
    {
        if (Machines[i][machine] == -1)
        {
            slots++;
        }
    }

    if (slots == jobDuration)
    {
        for (int i = time; i < time + jobDuration; i++)
        {
            Machines[i][machine] = jobNr;
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

// job0 (m0,3) m(1,2) m(2,2)
// job1 m(0,2) m(2,1) m(1,4)
// job2 m(1,4) m(2,3)  -----

// Escreve a job solution
//  Job 0: m0 t3	m1 t2	m2 t2
//  Job 1: m0 t2	m2 t1	m1 t4
//  Job 2: m1 t4	m2 t3	m-1 t-1
void PrintJobs(int y, int x, int totalTime)
{
    printf("\t");
    for (int i = 0; i < x; i++)
    {
        printf(" O-%d\t", i);
    }

    printf("\n");

    for (int i = 0; i < y; i++)
    {
        printf("Job %d: ", i);

        for (int j = 0; j < x; j++)
            printf(" %d\t", jobSolution[i][j]);
        printf("\n");
    }
    printf("Total time: %d \n", totalTime);
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

// Função para a leitura dos ficheiros no formato Fisher and Thompson para as matrizes globais
void ReadFile(char *fileName)
{

    // Abertura do ficheiro para leitura
    FILE *fpInput = fopen(fileName, "r");
    if (!fpInput)
    {
        perror("fopen");
        return;
    }

    // buffer de leitura
    char buffer[2048];

    int x = 0, y = 0;

    // Leitura da primeira linha que indica o tamanho da matriz
    fscanf(fpInput, "%d %d", &maxJobs, &maxMac);
    printf("Matrix size:\nNr lines:%d\nNr colums:%d\n", maxJobs, maxMac);

    // Leitura do restante ficheiro para o buffer
    fread(buffer, sizeof(*buffer), ARRAY_SIZE(buffer), fpInput);

    // Caracter de serapação dos numeros
    char *delim = " \r\n";

    // printf("buffer: %s\n", buffer);
    //  Função que parte uma string segundo um separador
    char *ptr = strtok(buffer, delim);

    int i = 0, nr = 0, job = 0, time = 0, man = 0;

    // Verificar
    //  ptr = strtok(NULL, delim);

    // Leitura dos caracteres separados
    while (ptr != NULL)
    {
        // Conversão para inteiro
        nr = atoi(ptr);
        // printf("%d ", nr);

        // Reset dos contadores para se passar para outra linha da matriz
        if (i == maxMac * 2)
        {

            man = 0;
            time = 0;
            i = 0;
            job++;
        }

        // Preenchimento das matrizes segundo a iteração que estamos for par ou impar
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

        // Chamada da função para devolver outra string
        ptr = strtok(NULL, delim);
        i++;
    }

    // printf("\n");
    // printf("%d %d\n", maxJobs, maxMac);

    for (int i = 0; i < maxJobs; i++)
    {
        printf("job:%d ", i);
        for (int j = 0; j < maxMac; j++)
        {
            printf("M:%d ", jobM[i][j]);
            printf("T:%d ", jobTime[i][j]);
        }
        printf("\n");
    }

    // Fechar o ficheiro
    fclose(fpInput);
}

// Função para escrita da solução num ficheiro
void WriteFile(char *filename, int maxJobs, int maxMac, int jobSolution[maxJobs][maxMac])
{
    // TODO file writing
}

int JobShop()
{
    // long nrThread = (long)rank;
    // int myShare = maxJobs / thread_count;
    // int y = 0, x = 0, max = 0, machine, job = y, time = 0;
    // int maxJ = nrThread * myShare + myShare;

    // printf("t:%ld y:%d j:%d share:%d\n", nrThread, y, maxJ, myShare);

    int time = 0;
#pragma omp parallel
    {
        int myShare = maxJobs / omp_get_num_threads();
        int t = omp_get_thread_num();
        int y = t * myShare, x = 0, machine, job = y, max = 0;
        int maxJ = (t + 1) * myShare + myShare + 1;

        printf("thread:%d y:%d maxJ:%d\n", t, y, maxJ);
        while (y < maxJ)
        {
#pragma omp critical
            {
                if (jobSolution[job][x] == -1 && verifyPreviousOp(job, x))
                {
                    machine = jobM[job][x];
                    if (Machines[time][machine] == -1)
                    {
                        if (max < jobTime[job][x])
                            max = jobTime[job][x];
                        InsertJob(machine, time, jobTime[job][x], job);
                        jobSolution[job][x] = time;
                    }
                }
            }

            // O ciclo percorre da esquerda para a direita e simultaneamente percorre a coluna de cada operação
            //  Confições de progressão do ciclo
            if (job == maxJ - 1)
            {
                x++;
                job = y;
                if (x == maxMac)
                {
                    y++;
                    x = 0;
                }
                if (max != 0)
                {
                    time = time + max;
                    max = 0;
                }
            }
            else
            {
                job++;
            }
        }
    }
    return time;
}

int VerifySolution()
{
    for (int y = 0; y < maxJobs; y++)
        for (int x = 0; x < maxMac; x++)
        {

            if (x > 1 && jobSolution[y][x - 1] > jobSolution[y][x])
            {
                return 1;
            }
        }

    return 0;
}