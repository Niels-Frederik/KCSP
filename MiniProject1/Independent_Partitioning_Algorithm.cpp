#include "Independent_Partitioning_Algorithm.h"
#include<thread>
#include<vector>
#include<math.h>

using namespace std;

vector<std::thread> threadList;

static uint32_t multiplicativeHash(int x, int hashBitNumber) {
    const std::uint32_t knuth = 2654435769;
    const std::uint32_t y = x;
    return (y * knuth) >> (32 - hashBitNumber);
}

void Independent_Partitioning_Algorithm::IndependentProcess(vector<tuple<long long, long long> >* tuples, int from, int to, int hashBits)  {
    int partitions = pow (2, hashBits);
    //vector<tuple<int,int> >* bufferArr[partitions];
    vector<vector<tuple<long long, long long>>> bufArr;
    for(int i = 0; i < (partitions); i++)
    {
        //vector<tuple<int,int> >* vect = new vector<tuple<int,int> >;
        vector<tuple<long long, long long>> vect;
        // should it be (to-from)/partitions?
        int expectedSize = to-from/partitions;
        //vect->reserve(expectedSize + expectedSize / 2);
        //bufferArr[i] = vect;
        bufArr.emplace_back(vect);
        bufArr[i].reserve(expectedSize + expectedSize / 2);
    }

    for (int i = from; i < to; i++)
    {
        auto element = (*tuples)[i];
        int partition = multiplicativeHash(get<0>(element), hashBits);
        //bufferArr[partition]->emplace_back(element);
        bufArr[partition].emplace_back(element);
    }
    int s = 1;
}

void Independent_Partitioning_Algorithm::IndependentPartition(vector<tuple<long long, long long> >* tuples, int threadCount, int hashBits) {
    int amountInEach = tuples->size()/threadCount;
    int previousLast = 0;
    while(threadCount--)
    {
        int to = previousLast+amountInEach;
        threadList.emplace_back(thread(IndependentProcess, tuples, previousLast, to, hashBits));
        previousLast = to;
    }

    for (auto& th : threadList)
    {
        th.join();
    }
    threadList.clear();
}
