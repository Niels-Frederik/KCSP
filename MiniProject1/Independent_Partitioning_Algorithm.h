#include<vector>
#include<tuple>
#include<future>
#ifndef MINIPROJECT1_INDEPENDENT_PARTITIONING_ALGORITHM_H
#define MINIPROJECT1_INDEPENDENT_PARTITIONING_ALGORITHM_H

using namespace std;

class Independent_Partitioning_Algorithm {
public:
    void IndependentPartition(vector<tuple<long long, long long> >* tuples, int threadCount, int hashBits);
    static void IndependentProcess(vector<tuple<long long, long long> >* tuples, int from, int to, int hashBits);
};


#endif
