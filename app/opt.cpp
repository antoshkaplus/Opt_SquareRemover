#include <iostream>
#include <fstream>

#include "tbb/tbb.h"
#include "tbb/parallel_for.h"

#include "optimizer.hpp"
#include "optimizer_v2.hpp"

using namespace std;
using namespace ant;

int main(int argc, const char * argv[])
{
    tbb::task_scheduler_init init;
    beam_v2::CheckMinSqMoves();
}