#include<tuple>
#include<vector>
#ifndef MINIPROJECT1_TUPLEGENERATOR_H
#define MINIPROJECT1_TUPLEGENERATOR_H

using namespace std;

class TupleGenerator {

public:
    vector<tuple<long long, long long> > GenerateTuples(int count);
};

#endif
