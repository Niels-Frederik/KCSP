#include "TupleGenerator.h"
#include "Concurrent_Partitioning_Algorithm.h"

using namespace std;

int main()
{
    TupleGenerator tg;
    Concurrent_Partitioning_Algorithm cpa;
    cpa.Partition(tg.GenerateTuples(1000000), 4);
}
