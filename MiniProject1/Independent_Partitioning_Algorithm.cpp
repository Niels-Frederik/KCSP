#include "Independent_Partitioning_Algorithm.h"
#include<thread>
#include<vector>
#include<iostream>
#include<future>
#include<random>
#include<math.h>

using namespace std;

vector<std::thread> threadList;

//hashbitnumber must be between 1 and 32
uint32_t multiplicativeHash(int x, int hashBitNumber) {
    const std::uint32_t knuth = 2654435769;
    const std::uint32_t y = x;
    return (y * knuth) >> (32 - hashBitNumber);
}

void Independent_Partitioning_Algorithm::IndependentProcess(vector<tuple<int,int>> tuples, int from, int to, int hashBits)  {
    int partitions = pow (2, hashBits);
    //tuple<int,int> buffer[to-from+1];
    //If the stack is large enough on the test computer, we can get away with the above
    vector<tuple<int,int>>* bufferArr[partitions];
    for(int i = 0; i < (partitions); i++)
    {
        vector<tuple<int,int>>* vect = new vector<tuple<int,int>>;
        vect->reserve(to-from/partitions);
        bufferArr[i] = vect;
    }

    for (int i = from; i < to; i++)
    {
        int partition = multiplicativeHash(get<0>(tuples[i]), hashBits);
        bufferArr[partition]->emplace_back(tuples[i]);
    }
}

void Independent_Partitioning_Algorithm::IndependentPartition(vector<tuple<int, int>> tuples, int threadCount, int hashBits) {
    int amountInEach = tuples.size()/threadCount;
    int previousLast = 0;
    while(threadCount--)
    {
        int to = previousLast+amountInEach;
        cout << "start thread " << " "  << "from " << previousLast << " to " << to << endl;
        threadList.emplace_back(thread(IndependentProcess, tuples, previousLast, to, hashBits));
        previousLast = to;
    }

    for (auto& th : threadList)
    {
        th.join();
    }
}
