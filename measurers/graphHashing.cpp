#include "../realization/graphHashing.h"

signed main() {

    // стресс тестирование (в stdout появятся записи вида Failed for p={} size={})

    ofstream testing;
    vector<int> test =  {50, 1000, 5000, 10000, 50000, 100000};
    measure<int, GraphHashing<int, int, HashFunctionType1<int>>>(generateVectorInt, testing, test);
    measure<string, GraphHashing<string, int, HashFunctionType1<string>>>(generateVectorString, testing, test);

    // для графиков

//    ofstream intOut("../results/graphHashing/int.out");
//
//    measure<int, GraphHashing<int, int, HashFunctionType2<int>>>(generateVectorInt, intOut);
//
//    intOut.close();
//
//    ofstream stringOut("../results/graphHashing/string.out");
//
//    measure<string, GraphHashing<string, int, HashFunctionType2<string>>>(generateVectorString, stringOut);
//
//    stringOut.close();

    return 0;
}
