#include<tuple>
#include<vector>
#include<random>
#include<limits.h>
#include<thread>
#include<atomic>
#include<algorithm>
#include<iostream>
#include<mutex>
#include<array>
using namespace std;

vector<tuple<long long, long long>> GenerateData(int count, int hashBits)
{
    vector<tuple<long long, long long>> result;
    result.reserve(count);

    const long long range_from = 0;
    const long long range_to = LLONG_MAX;
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<long long> dis(range_from, range_to);
    for (int i = 0; i < count; i++)
    {
        long long key = dis(gen) % hashBits;
        long long data = dis(gen);
        //result.emplace_back(make_tuple(dis(gen), dis(gen)));
        result.emplace_back(make_tuple(key, data));
    }
    return result;
}

//==================================== CONCURRENT =====================================================================

void ConcurrentRun(const vector<tuple<long long, long long>>* tuples, vector<vector<tuple<long long, long long>>>* buffer, int from, int to, array<atomic<size_t>, 262144>* arr, int hashBits)
{
    for (int i = from; i < to; i++)
    {
        auto element = (*tuples)[i];
        auto key = get<0>(element);
        int writeIndex = (*arr)[key]++;
        (*buffer)[key][writeIndex] = element;
    }
}

void Concurrent(const vector<tuple<long long, long long>>* tuples, int threadCount, int hashBits, int amountInEach)
{
    vector<thread> threads;
    vector<vector<tuple<long long, long long>>> buffer;

    array<atomic_size_t, 262144> arr{};
    int expectedSize = tuples->size()/hashBits;

    for(int i = 0; i < hashBits; i++)
    {
        vector<tuple<long long, long long>> vect;
        buffer.emplace_back(vect);
        // Allocate 50% more than the expected need
        buffer[i].reserve(expectedSize + expectedSize * 0.5);
    }

    int previousLast = 0;
    while(threadCount--)
    {
        int to = previousLast+amountInEach;
        threads.emplace_back(thread(ConcurrentRun, tuples, &buffer, previousLast, to, &arr, hashBits));
        previousLast = to;
    }
    //wait for execution
    for (auto& th : threads)
    {
        th.join();
    }
}

//==================================== INDEPENDENT =====================================================================

void IndependentRun(const vector<tuple<long long, long long>>* tuples, int from, int to, int hashBits)  {
    vector<vector<tuple<long long, long long>>> buffer;
    for(int i = 0; i < hashBits; i++)
    {
        vector<tuple<long long, long long>> vect;
        int expectedSize = to-from/hashBits;
        vect.reserve(expectedSize + expectedSize * 0.5);
        buffer.emplace_back(vect);
        //buffer[i].reserve(expectedSize + expectedSize*1);
    }

    for (int i = from; i < to; i++)
    {
        auto element = (*tuples)[i];
        buffer[get<0>(element)].emplace_back(element);
    }
}

void Independent(const vector<tuple<long long, long long>>* tuples, int threadCount, int hashBits, int amountInEach) {
    vector<thread> threads;
    int previousLast = 0;
    while(threadCount--)
    {
        int to = previousLast+amountInEach;
        (threads).emplace_back(thread(IndependentRun, tuples, previousLast, to, hashBits));
        previousLast = to;
    }

    //wait for execution
    for (auto& th : threads)
    {
        th.join();
    }
    //threads.clear();
}

int main(int argc, char** argv)
{
    const int ALGORITHM = atoi(argv[1]);
    const int TUPLES = atoi(argv[2]);
    const int THREADS = atoi(argv[3]);
    const int HASHBITS = pow(2, atoi(argv[4]));


    const vector<tuple<long long, long long>> tuples = GenerateData(TUPLES, HASHBITS);
    int amountInEach = tuples.size()/THREADS;
    auto start = std::chrono::high_resolution_clock::now();
    if(ALGORITHM == 0)
    {
        Concurrent(&tuples, THREADS, HASHBITS, amountInEach);
    }
    else
    {
        Independent(&tuples, THREADS, HASHBITS, amountInEach);
    }

    //print running time
    auto finish = std::chrono::high_resolution_clock::now();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(finish-start);
    auto throughput = (TUPLES/(milliseconds.count())*1000)/1000000;
    cout << throughput << endl;
    //cout << microseconds.count() << endl;
}
