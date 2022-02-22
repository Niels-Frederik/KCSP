#include<tuple>
#include<vector>
#ifndef MINIPROJECT1_TUPLEGENERATOR_H
#define MINIPROJECT1_TUPLEGENERATOR_H

using namespace std;

class TupleGenerator {

public:
    vector<tuple<int,int>> GenerateTuples(int count);
};

#endif
