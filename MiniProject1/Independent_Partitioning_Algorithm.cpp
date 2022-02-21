#include "Independent_Partitioning_Algorithm.h"
#include<thread>
#include<vector>
#include<iostream>
#include<future>
#include<random>

using namespace std;

vector<std::thread> threadList;

void Independent_Partitioning_Algorithm::IndependentProcess(vector<tuple<int,int>> tuples, int from, int to)  {
    //tuple<int,int> buffer[to-from+1];
    tuple<int,int>* buffer = new tuple<int,int>[to-from];

    int writeIndex = 0;
    for (int i = from; i < to; i++)
    {
        tuple<int,int> tempValueForVisualization = make_tuple(i,i);
        buffer[writeIndex] = tempValueForVisualization;
        writeIndex++;
    }
}

void Independent_Partitioning_Algorithm::IndependentPartition(vector<tuple<int, int>> tuples, int threadCount) {
    int amountInEach = tuples.size()/threadCount;
    int previousLast = 0;
    while(threadCount--)
    {
        int to = previousLast+amountInEach;
        cout << "start thread " << " "  << "from " << previousLast << " to " << to << endl;
        threadList.emplace_back(thread(IndependentProcess, tuples, previousLast, to));
        previousLast = to;
    }

    for (auto& th : threadList)
    {
        th.join();
    }
}
