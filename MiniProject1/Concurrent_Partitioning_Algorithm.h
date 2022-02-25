#include<vector>
#include<tuple>
#ifndef MINIPROJECT1_CONCURRENT_PARTITIONING_ALGORITHM_H
#define MINIPROJECT1_CONCURRENT_PARTITIONING_ALGORITHM_H

using namespace std;


class Concurrent_Partitioning_Algorithm {
public:
    void ConcurrentPartition(vector<tuple<int,int> > tuples, int threadCount, int hashBits);
    static void Process(vector<tuple<int,int> > tuples, int from, int to, int hashBits);
};

#endif
