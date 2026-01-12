#include "lab1_IO.h"
#include "timer.h"
#include "demo.h"
#include <pthread.h>
#include <math.h>
#include <stdlib.h>

// Matrix Pointers
int **A;
int **B;
int **C;
int n;

pthread_t *threadList;
Block *blocks;

// Need to come up with a better name for "number_of_blocks" since this function returns ---> the k value where k * k == n ;
int compute_number_of_blocks_from_N(int n)
{
    // Limiting the max size of matrix C to sqrt(N)
    int k = (int)sqrt(n);

    // Find a value of k that breaks N into equal parts
    while (k > 1)
    {
        if (n % k == 0)
            break;

        k--;
    }
    return k;
}

void multiplyMatrix(Block *block_partition)
{
    int last_row_pos = block_partition->start_row + block_partition->height;
    int last_column_pos = block_partition->start_col + block_partition->width;

    // NON - Parallelized Solution:
    for (int i = block_partition->start_row; i < last_row_pos; i++) // Lock Row for C
    {
        for (int j = block_partition->start_col; j < last_column_pos; j++) // Lock Col for C
        {
            block_partition->C[i][j] = 0;

            for (int k = 0; k < block_partition->n; k++)
            {
                // Since both matrices are n x n -> K == n.
                // Therefore we can traverse a whole column top to bottom and a whole row left to right from Matrices B and A respectively.
                block_partition->C[i][j] += block_partition->A[i][k] * block_partition->B[k][j];
            }
        }
    }
}

void *threadfunc(void *arg_p)
{
    Block *block = (Block *)arg_p;
    multiplyMatrix(block);
    return NULL;
}

void createThreads(int number_of_blocks)
{
    int num_threads = number_of_blocks * number_of_blocks;

    threadList = malloc(num_threads * sizeof(pthread_t));
    blocks = malloc(num_threads * sizeof(Block));

    int block_height = n / number_of_blocks;
    int block_width = n / number_of_blocks;

    for (int thread_count = 0; thread_count < num_threads; thread_count++)
    {
        int block_row = thread_count / number_of_blocks;
        int block_col = thread_count % number_of_blocks;

        blocks[thread_count].n = n;
        blocks[thread_count].A = A;
        blocks[thread_count].B = B;
        blocks[thread_count].C = C;

        blocks[thread_count].height = block_height;
        blocks[thread_count].width = block_width;

        blocks[thread_count].start_col = block_col * block_width;
        blocks[thread_count].start_row = block_row * block_height;

        pthread_create(&threadList[thread_count], NULL, threadfunc, &blocks[thread_count]);
    }
}

void joinThreads(int number_of_blocks)
{
    int num_threads = number_of_blocks * number_of_blocks;
    for (int thread_count = 0; thread_count < num_threads; thread_count++)
    {
        pthread_join(threadList[thread_count], NULL);
    }
    return;
}

void deallocateMem()
{
    for (int i = 0; i < n; i++)
    {
        free(C[i]);
    }
    free(C);

    free(threadList);
    free(blocks);

    return;
}

int main(int argc, char *argv[])
{
    double start_time = 0;
    double end_time = 0;

    // Load Matrix
    Lab1_loadinput(&A, &B, &n);

    // Start Timer
    GET_TIME(start_time);

    // Perform Matrix Multiplication
    C = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        C[i] = malloc(n * sizeof(int));
    }

    int number_of_blocks = compute_number_of_blocks_from_N(n);

    createThreads(number_of_blocks);

    joinThreads(number_of_blocks);

    // End Timer
    GET_TIME(end_time);

    // Save new Matrix
    Lab1_saveoutput(C, &n, end_time - start_time);

    deallocateMem();

    return 0;
}