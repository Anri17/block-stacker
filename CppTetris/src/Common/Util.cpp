#include "Util.h"

#include <chrono>

int Util::Random(int range)
{
    srand(time(NULL));
    return rand() % range;
}
