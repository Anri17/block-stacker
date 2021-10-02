#include "Util.h"

#include <chrono>

int Util::Random(int range)
{
    srand(time(NULL));

    return rand() % range;
}

int Util::Random(int low, int high)
{
    int difference = high - low;

    srand(time(NULL));

    return (rand() % difference) + low;
}
