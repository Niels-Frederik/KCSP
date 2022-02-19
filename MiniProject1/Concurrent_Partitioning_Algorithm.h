#include<vector>
#include<tuple>
#ifndef MINIPROJECT1_CONCURRENT_PARTITIONING_ALGORITHM_H
#define MINIPROJECT1_CONCURRENT_PARTITIONING_ALGORITHM_H

using namespace std;


class Concurrent_Partitioning_Algorithm {
public:
    void Partition(vector<tuple<int,int>> tuples, int threadCount);
};


#endif
