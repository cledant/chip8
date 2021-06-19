#include "tools.h"

#include <time.h>

double
tool_get_time()
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return (ts.tv_sec + (ts.tv_nsec / 1000000000.0));
}
