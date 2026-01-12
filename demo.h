#include <pthread.h>

#ifndef DEMO
#define DEMO

typedef struct
{
    int n;
    int height;
    int width;
    int start_row;
    int start_col;
    int **A;
    int **B;
    int **C;
} Block;

/*******************************************************************
 * threadfunc
 *
 * Description:
 * Describes the Function that is provided to a thread upon creation
 *
 * Parameters:
 * arg_p -
 *
 * Returns:
 *
 *
 * Side Effects:
 * None.
 *******************************************************************/
void *threadfunc(void *arg_p);

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
int compute_number_of_blocks_from_N(int n);

#endif
