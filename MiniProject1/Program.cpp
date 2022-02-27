#include<tuple>
#include<vector>
#include<random>
#include<limits.h>
#include<thread>
#include<atomic>
#include<algorithm>
#include<iostream>
#include<mutex>
#include<barrier>
#include<latch>
#include<array>
using namespace std;

//1 => 2^1-1 = 0-1 (2)
//2 => 2^2-1 = 0-3 (4)
//3 => 2^3-1 = 0-7 (8)
//4 => 2^4-1 = 0-15 (16)
//5 => 2^5-1 = 0-31 (32)

//between 0 and 2^hashBitNumber-1
static uint32_t Hash(const tuple<long long, long long>& x, int hashBitNumber) {
    auto element = get<0>(x);
    const std::uint32_t knuth = 2654435769;
    const std::uint32_t y = element;
    return (y * knuth) >> (32 - hashBitNumber);
}

vector<tuple<long long, long long>> GenerateData(int count)
{
    vector<tuple<long long, long long>> result;
    result.reserve(count);
    random_device rd;  // Will be used to obtain a seed for the random number engine
    // Payload needs to be 16 bytes (tuple of long long)
    mt19937_64 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    uniform_int_distribution<> dis(0,LLONG_MAX);
    for (int i = 0; i < count; i++)
    {
        result.emplace_back(make_tuple(dis(gen), dis(gen)));
    }
    return result;
}

//==================================== CONCURRENT =====================================================================

void ConcurrentRun(const vector<tuple<long long, long long>>* tuples, vector<vector<tuple<long long, long long>>>* buffer, int from, int to, vector<mutex>& locks, int hashBits)
{
    for (int i = from; i < to; i++)
    {
        auto element = (*tuples)[i];
        uint32_t hash = Hash(element, hashBits);
        std::lock_guard<std::mutex> guard(locks[hash]);
        (*buffer)[hash].emplace_back(element);
    }

    auto x = 1;
}

void Concurrent(const vector<tuple<long long, long long>>* tuples, int threadCount, int hashBits, int amountInEach)
{
    vector<thread> threads;
    vector<vector<tuple<long long, long long>>> buffer;
    const int partitions = pow(2, hashBits);
    vector<mutex> locks;
    locks = vector<mutex>(partitions+1);

    for(int i = 0; i < partitions; i++)
    {
        vector<tuple<long long, long long>> vect;
        int expectedSize = tuples->size()/partitions;
        buffer.emplace_back(vect);
        // Allocate 50% more than the expected need
        buffer[i].reserve(expectedSize + expectedSize / 2);
    }

    int previousLast = 0;
    while(threadCount--)
    {
        int to = previousLast+amountInEach;
        threads.emplace_back(thread(ConcurrentRun, tuples, &buffer, previousLast, to, ref(locks), hashBits));
        previousLast = to;
    }

    //wait for execution
    for (auto& th : threads)
    {
        th.join();
    }
    threads.clear();
}

//==================================== INDEPENDENT =====================================================================

void IndependentRun(const vector<tuple<long long, long long>>* tuples, int from, int to, int hashBits)  {
    int partitions = pow (2, hashBits);
    vector<vector<tuple<long long, long long>>> buffer;
    for(int i = 0; i < (partitions); i++)
    {
        vector<tuple<long long, long long>> vect;
        int expectedSize = to-from/partitions;
        buffer.emplace_back(vect);
        buffer[i].reserve(expectedSize + expectedSize / 2);
    }

    for (int i = from; i < to; i++)
    {
        auto element = (*tuples)[i];
        int partition = Hash(element, hashBits);
        buffer[partition].emplace_back(element);
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
    threads.clear();
}

int main(int argc, char** argv)
{
    const int ALGORITHM = atoi(argv[1]);
    const int TUPLES = atoi(argv[2]);
    const int THREADS = atoi(argv[3]);
    const int HASHBITS = atoi(argv[4]);

    const vector<tuple<long long, long long>> tuples = GenerateData(TUPLES);
    cout << "Done generating tuples" << endl;
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
    auto microseconds = std::chrono::duration_cast<std::chrono::milliseconds>(finish-start);
    cout << microseconds.count() << "ms\n";
}
