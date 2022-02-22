#include "Concurrent_Partitioning_Algorithm.h"
#include<vector>
#include<tuple>
#include<thread>
#include<atomic>
#include<iterator>
#include <random>

using namespace std;

vector<tuple<int,int>>** bufferArr;
vector<std::thread> threads;
atomic_int** writeLockArr;

static uint32_t hashing(int x, int hashBitNumber) {
    const std::uint32_t knuth = 2654435769;
    const std::uint32_t y = x;
    return (y * knuth) >> (32 - hashBitNumber);
}

void Concurrent_Partitioning_Algorithm::Process(vector<tuple<int,int>> tuples, int from, int to, int hashBits)  {
    for (int i = from; i < to; i++)
    {
        uint32_t hash = hashing(get<0>(tuples[i]), hashBits);

        auto writeIndex = writeLockArr[hash]->fetch_add(1);
        (*bufferArr[hash])[writeIndex] = tuples[i];
    }
}

void Concurrent_Partitioning_Algorithm::ConcurrentPartition(vector<tuple<int, int>> tuples, int threadCount, int hashBits) {
    int partitions = pow(2, hashBits);

    bufferArr = new vector<tuple<int,int>>*[partitions];
    writeLockArr = new atomic_int*[partitions];

    for(int i = 0; i < partitions; i++)
    {
        writeLockArr[i] = new atomic_int (0);
        bufferArr[i] = new vector<tuple<int,int>>;
        bufferArr[i]->reserve(tuples.size()/partitions+(tuples.size() * 0.1));
    }

    int amountInEach = tuples.size()/threadCount;
    int previousLast = 0;
    while(threadCount--)
    {
        int to = previousLast+amountInEach;
        threads.emplace_back(thread(Process, tuples, previousLast, to, hashBits));
        previousLast = to;
    }

    for (auto& th : threads)
    {
        th.join();
    }
}

