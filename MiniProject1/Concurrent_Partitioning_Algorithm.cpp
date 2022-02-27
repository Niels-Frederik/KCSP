#include "Concurrent_Partitioning_Algorithm.h"
#include<vector>
#include<tuple>
#include<thread>
#include<atomic>
#include<iterator>
#include<random>

using namespace std;

vector<std::thread> threads;
vector<vector<tuple<long long, long long>>> bufArr;
vector<mutex> locks;
//mutex* locks;

static uint32_t hashing(const tuple<long long, long long>& x, int hashBitNumber) {
    auto element = get<0>(x);
    const std::uint32_t knuth = 2654435769;
    const std::uint32_t y = element;
    return (y * knuth) >> (32 - hashBitNumber);
}

void Concurrent_Partitioning_Algorithm::Process(vector<tuple<long long, long long>>* tuples, int from, int to, int hashBits)  {
    for (int i = from; i < to; i++)
    {
        auto element = (*tuples)[i];
        uint32_t hash = hashing(element, hashBits);
        std::lock_guard<std::mutex> guard(locks[hash]);
        bufArr[hash].emplace_back(element);
    }
}

void Concurrent_Partitioning_Algorithm::ConcurrentPartition(vector<tuple<long long, long long>>* tuples, int threadCount, int hashBits) {
    int partitions = pow(2, hashBits);
    threads.reserve(threadCount);
    //locks = new mutex[partitions];
    locks = vector<mutex>(partitions);

    for(int i = 0; i < partitions; i++)
    {
        vector<tuple<long long, long long>> vect;
        // Allocate 50% more than the expected need
        int expectedSize = tuples->size()/partitions;
        //vect.reserve(expectedSize + expectedSize / 2);
        bufArr.emplace_back(vect);
        bufArr[i].reserve(expectedSize + expectedSize / 2);
    }

    int amountInEach = tuples->size()/threadCount;
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

    threads.clear();
    //delete(locks);
}

