#include<iostream>
#include <random>

using namespace std;

vector<tuple<int,int>> GenerateTuples(int count){
    vector<tuple<int,int>> result;
    random_device rd;  // Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    uniform_int_distribution<> dis(INT32_MIN, INT32_MAX);
    for (int i = 0; i < count; i++)
    {
        result.emplace_back(make_tuple(dis(gen), dis(gen)));
    }
    return result;
}

int main()
{
    vector<tuple<int,int>> lst = GenerateTuples(5);
    for(tuple<int,int> tup : lst){
        cout << get<0>(tup) << ", " << get<1>(tup) << endl;
    }
}