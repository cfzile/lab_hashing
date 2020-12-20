#include "../realization/twoTieredApproach.h"

signed main() {

    // стресс тестирование (в stdout появятся записи вида Failed for p={} size={})

//    ofstream testing;
//    vector<int> test =  {100000};
//    measure<int, TwoTieredApproach<int, int, HashFunctionType1<int>>>(generateVectorInt, testing, test);
//    measure<string, TwoTieredApproach<string, int, HashFunctionType1<string>>>(generateVectorString, testing, test);

    //

    ofstream intOut("../results/twoTieredApproach/int.out");

    measure<int, TwoTieredApproach<int, int, HashFunctionType1<int>>>(generateVectorInt, intOut);

    intOut.close();

    ofstream stringOut("../results/twoTieredApproach/string.out");

    measure<string, TwoTieredApproach<string, int, HashFunctionType1<string>>>(generateVectorString, stringOut);

    stringOut.close();

    readRealData();

    ofstream realOut("../results/twoTieredApproach/real.out");

    measure<string, TwoTieredApproach<string, int, HashFunctionType1<string>>>(getRealData, realOut);

    realOut.close();

    return 0;
}
