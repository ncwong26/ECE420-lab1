#include <pthread.h>

#ifndef DEMO
#define DEMO

typedef struct
{
    int thread_id;
    int start_row;
    int start_col;
} MatrixThread;

typedef struct
{
    int n;
    int height;
    int width;
    int thread_id;
    int start_row;
    int start_col;
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

#endif
