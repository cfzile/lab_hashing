#include "../realization/hashFunctions.h"
#include "../realization/idealHashing.h"
#include "../realization/graphHashing.h"

struct result{
    bool processedSuccessful;
    double hashingTime;
    double averageSearchTime;
    double processedMemory;
};

template <typename T, typename D, typename H>
result hashing(vector<pair<T, D>> &v, int p = 1) {
    double vm, rss;
    process_mem_usage(vm, rss);

    auto timeHashingBegin = chrono::system_clock::now();
    auto idealHashing = new IdealHashing<T, D, H>(v, p * v.size() * v.size());
    double hashingTime = (double)chrono::duration_cast<std::chrono::microseconds>(chrono::system_clock::now() - timeHashingBegin).count();

    double vm2, rss2;
    process_mem_usage(vm2, rss2);

    double processedMemory = vm2 - vm;

    bool processedSuccessful = idealHashing->success;

    int idealHashingFalse = 0;
    auto averageSearchTimeBegin = chrono::system_clock::now();
    for (auto i : v) {
        if (idealHashing->search(i.first) != i.second) {
           processedSuccessful = false;
        }
    }
    double averageSearchTime = (double)chrono::duration_cast<std::chrono::microseconds>(chrono::system_clock::now() - averageSearchTimeBegin).count()/v.size();

    return result{processedSuccessful, hashingTime, averageSearchTime, processedMemory};
}

template<typename T>
void measure(vector<pair<T, int>> (*generateFunction)(long long), ofstream & out) {
    vector<int> sizes = {100, 500, 1000, 5000};

    int attemptsNum = 5;

    for (int p = 1; p <= 4; p += 1) {
        for (auto size : sizes) {
            double hashingTime = 0;
            double averageSearchTime = 0;
            double processedMemory = 0;
            bool processedSuccessful = true;

            vector<pair<T, int>> v = generateFunction(size);
            for (int attempt = 0; attempt < attemptsNum; attempt += 1) {
                result res = hashing<T, int, HashFunctionType1<T>>(v, p);
                hashingTime += res.hashingTime;
                averageSearchTime += res.averageSearchTime;
                processedMemory += res.processedMemory;
                processedSuccessful = processedSuccessful && res.processedSuccessful;
            }
            hashingTime /= attemptsNum;
            averageSearchTime /= attemptsNum;
            processedMemory /= attemptsNum;

            if (processedSuccessful) {
                out << "p=" << p << " size=" << size << " avg_hashingTime="
                    << hashingTime << " avg_averageSearchTime=" << averageSearchTime
                    << " avg_processedMemory=" << processedMemory << "\n";
            } else {
                cout << "Failed for p=" << p << " size=" << size << "\n";
            }
        }
    }

}

signed main() {

    ofstream intOut("../results/idealHashing/intOut");

    measure<int>(generateVectorInt, intOut);

    intOut.close();


    ofstream stringOut("../results/idealHashing/stringOut");

    measure<string>(generateVectorString, stringOut);

    stringOut.close();

    return 0;
}
