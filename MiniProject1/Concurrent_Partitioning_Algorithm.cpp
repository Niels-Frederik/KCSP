#include "Concurrent_Partitioning_Algorithm.h"
#include<vector>
#include<tuple>
#include<thread>
#include<iostream>
#include<atomic>
#include<iterator>
#include <random>

using namespace std;

//vector<int> buffer;
tuple<int,int>* buffer;
vector<std::thread> threads;
atomic<int> writeIndex(0);

int hashFunc(tuple<int,int> tuple) {
    //return (get<0>(tuple) * get<1>(tuple));
    return (420);
}

void process(vector<tuple<int,int>> tuples, int from, int to)  {
    for (int i = from; i < to; i++)
    {
        int val1 = get<0>(tuples[i]);
        int val2 = get<1>(tuples[i]);
        //int hash = val1 % val2;
        //int result = hashFunc(tuples[i]);
        tuple<int,int> tempValueForVisualization = make_tuple(i,i);
        //int sleepTime = 10000L+(long)((1e5-1e4)*rand()/(RAND_MAX+1.0));
        //this_thread::sleep_for(chrono::milliseconds(distr(gen)));
        buffer[writeIndex++] = tempValueForVisualization;
        //hash(tuples[i]);
        //buffer[writeIndex++] = result;
        //buffer.emplace_back(result);
    }
    cout << "thread done: " << thread::id() << endl;
}

void Concurrent_Partitioning_Algorithm::Partition(vector<tuple<int, int>> tuples, int threadCount) {

    //int buffer [tuples.size()];
    buffer = new tuple<int,int>[tuples.size()];
    int amountInEach = tuples.size()/threadCount;
    int previousLast = 0;
    while(threadCount--)
    {
        int to = previousLast+amountInEach;
        cout << "start thread " << " "  << "from " << previousLast << " to " << to << endl;
        threads.emplace_back(thread(process, tuples, previousLast, to));
        //thread t(process, tuples, previousLast, to);
        //threads.emplace_back(&t);
        previousLast = to;
    }

    for (auto& th : threads)
    {
        th.join();
    }

    for(int i = 0; i < tuples.size(); i++)
    {
        cout << get<0>(buffer[i]) << ", " << get<1>(buffer[i]) << endl;
    }
}

