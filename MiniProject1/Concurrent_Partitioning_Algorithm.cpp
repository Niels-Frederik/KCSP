#include "Concurrent_Partitioning_Algorithm.h"
#include<vector>
#include<tuple>
#include<thread>
#include<iostream>
#include<atomic>
#include<iterator>
#include <random>

using namespace std;

tuple<int,int>* buffer;
vector<std::thread> threads;
atomic<int> writeIndex(0);

int hashFunc(tuple<int,int> tuple) {
    return (420);
}

void Concurrent_Partitioning_Algorithm::Process(vector<tuple<int,int>> tuples, int from, int to)  {
    for (int i = from; i < to; i++)
    {
        tuple<int,int> tempValueForVisualization = make_tuple(i,i);
        buffer[writeIndex++] = tempValueForVisualization;
    }
}

void Concurrent_Partitioning_Algorithm::ConcurrentPartition(vector<tuple<int, int>> tuples, int threadCount) {

    buffer = new tuple<int,int>[tuples.size()];
    int amountInEach = tuples.size()/threadCount;
    int previousLast = 0;
    while(threadCount--)
    {
        int to = previousLast+amountInEach;
        threads.emplace_back(thread(Process, tuples, previousLast, to));
        previousLast = to;
    }

    for (auto& th : threads)
    {
        th.join();
    }
}

