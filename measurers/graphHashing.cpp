#include "../realization/graphHashing.h"

signed main() {

    ofstream intOut("../results/graphHashing/int.out");

    measure<int, GraphHashing<int, int, HashFunctionType1<int>>>(generateVectorInt, intOut);

    intOut.close();

    ofstream stringOut("../results/graphHashing/string.out");

    measure<string, GraphHashing<string, int, HashFunctionType1<string>>>(generateVectorString, stringOut);

    stringOut.close();

    return 0;
}
