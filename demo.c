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

Block *block_partition;

/*******************************************************************
 * compute_P_from_N
 *
 * Description:
 * Compute the number of equal sized blocks/partitions, K, from the a variable maatrix size, N
 * Which can be used to calculate the number of threads, P, which would be k * k = P;
 *
 * Parameters:
 * n - matrix size
 *
 * Returns:
 * k - Number of equal sized blocks/partitions of N
 *
 * Side Effects:
 * None.
 *******************************************************************/
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
            C[i][j] = 0;

            for (int k = 0; k < block_partition->n; k++)
            {
                // Since both matrices are n x n -> K == n.
                // Therefore we can traverse a whole column top to bottom and a whole row left to right from Matrices B and A respectively.
                C[i][j] += A[i][k] * B[k][j];
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

int main(int argc, char *argv[])
{

    block_partition = malloc(sizeof(Block));

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

    // int number_of_blocks = compute_number_of_blocks_from_N(n);

    block_partition->n = n;
    block_partition->height = n;
    block_partition->width = n;
    block_partition->start_col = 0;
    block_partition->start_row = 0;
    block_partition->thread_id = 0;

    multiplyMatrix(block_partition);

    // End Timer
    GET_TIME(end_time);

    // Save new Matrix
    Lab1_saveoutput(C, &n, end_time - start_time);

    for (int i = 0; i < n; i++)
    {
        free(C[i]);
    }
    free(C);
    free(block_partition);

    return 0;
}