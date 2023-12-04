#include "Util.h"

#include <chrono>

int Util::Random(int range)
{
    return rand() % range;
}

int Util::Random(int low, int high)
{
    int difference = high - low;

    return (rand() % difference) + low;
}
