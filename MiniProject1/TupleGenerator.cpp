#include "vector"
#include "tuple"
#include "TupleGenerator.h"
#include "random"

using namespace std;

vector<tuple<long long, long long>> TupleGenerator::GenerateTuples(int count) {
    vector<tuple<long long, long long>> result;
    result.reserve(count);
    random_device rd;  // Will be used to obtain a seed for the random number engine
    // Payload needs to be 16 bytes (tuple of long long)
    mt19937_64 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    uniform_int_distribution<> dis(0,ULONG_LONG_MAX);
    for (int i = 0; i < count; i++)
    {
        result.emplace_back(make_tuple(dis(gen), dis(gen)));
    }
    return result;
}
