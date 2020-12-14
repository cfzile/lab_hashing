#include "../realization/twoTieredApproach.h"

signed main() {

    ofstream intOut("../results/twoTieredApproach/int.out");

    measure<int, TwoTieredApproach<int, int, HashFunctionType1<int>>>(generateVectorInt, intOut);

    intOut.close();

    ofstream stringOut("../results/twoTieredApproach/string.out");

    measure<string, TwoTieredApproach<string, int, HashFunctionType1<string>>>(generateVectorString, stringOut);

    stringOut.close();

    return 0;
}
