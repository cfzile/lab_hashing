#include "../realization/idealHashing.h"

signed main() {

    ofstream intOut("../results/idealHashing/int.out");

    measure<int, IdealHashing<int, int, HashFunctionType1<int>, 8000001>>(generateVectorInt, intOut);

    intOut.close();

    ofstream stringOut("../results/idealHashing/string.out");

    measure<string, IdealHashing<string, int, HashFunctionType1<string>, 8000001>>(generateVectorString, stringOut);

    stringOut.close();

    return 0;
}
