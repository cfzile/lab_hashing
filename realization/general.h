#pragma once

#include <bits/stdc++.h>
#include <sys/resource.h>
#include <unistd.h>

long long inf = 20000000;

using namespace std;

vector<long long> primes = {2147483647, 587617, 96873151, 49782769, 25556317, 32287093, 84180703, 29502881, 60573089, 41091107, 19289551,
                      6114391, 9899173, 41257409, 5331379, 23706707, 98863543, 5092069, 74843921, 24824197, 52168027, 437321,
                      96505763, 264959, 5917883, 900917, 22072411, 22146139, 848921, 3848017, 297533, 862819, 28036597};

random_device rd;
mt19937 mt(rd());

int generateInteger(int a, int b) {
    uniform_int_distribution<int> dist(a, b);
    return dist(mt);
}

vector<int> generateVectorInteger(int size, int A, int B) {
    uniform_int_distribution<int> dist(A, B);
    vector<int> result;
    for (int i = 0; i < size; i++)
        result.push_back(dist(mt));
    return result;
}

vector<pair<int, int>> generateVectorInt(int size) {
    vector<pair<int, int>> result;
    set<int> used;
    for (int i = 0; i < size; i++) {
        int key = generateInteger(0, inf);
        while (used.find(key) != used.end()) {
            key = generateInteger(0, inf);
        }
        result.emplace_back(key, generateInteger(0, inf));
    }
    return result;
}

vector<pair<string, int>> generateVectorString(int size) {
    vector<pair<string, int>> result;
    set<string> keys;
    for (int i = 0; i < size; i++) {
        string str = "";
        do {
            int string_size = generateInteger(0, 15) + 1;
            str = "";
            for (int j = 0; j < string_size; j++) {
                str += (char) generateInteger(65, 127);
            }
        } while (keys.find(str) != keys.end());
        result.emplace_back(str, generateInteger(0, inf));
        keys.insert(str);
    }
    return result;
}

vector<pair<string, int>> realData;

void readRealData() {
    ifstream text1("../realdata/text1.txt");
    ifstream text2("../realdata/text2.txt");
    ifstream text3("../realdata/text3.txt");
    set<string> keys;
    string s;
    int num = 0;
    while (text1 >> s) {
        if (keys.find(s) != keys.end() || s.size() == 0)
            continue;
        realData.push_back({s, num++});
        keys.insert(s);
    }
    while (text2 >> s) {
        if (keys.find(s) != keys.end() || s.size() == 0)
            continue;
        realData.push_back({s, num++});
        keys.insert(s);
    }
    while (text3 >> s) {
        if (keys.find(s) != keys.end() || s.size() == 0)
            continue;
        realData.push_back({s, num++});
        keys.insert(s);
    }
    text1.close();
    text2.close();
    text3.close();
}

vector<pair<string, int>> getRealData(int size) {
    decltype(realData) data;
    for (auto i : realData){
        if (!size)
            break;
        data.push_back(i);
        size -= 1;
    }
    return data;
}

void process_mem_usage(double &vm_usage, double &resident_set) {
    vm_usage = 0.0;
    resident_set = 0.0;

    // the two fields we want
    unsigned long vsize;
    long rss;
    {
        std::string ignore;
        std::ifstream ifs("/proc/self/stat", std::ios_base::in);
        ifs >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
            >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
            >> ignore >> ignore >> vsize >> rss;
    }

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage = vsize / 1024.; // Кбайт
    resident_set = rss * page_size_kb;
}

struct result {
    bool processedSuccessful;
    double hashingTime;
    double averageSearchTime;
    double processedMemory;
    double numAttempts;
};

double getDiff(chrono::_V2::system_clock::time_point time) {
    return (double) chrono::duration_cast<std::chrono::microseconds>(chrono::system_clock::now() - time).count();
}

template<typename T, typename D, typename C>
result hashing(vector<pair<T, D>> &v, int p = 1) {
    double vm, rss;
    process_mem_usage(vm, rss);

    auto timeHashingBegin = chrono::system_clock::now();
    auto idealHashing = new C(v, p);
    double hashingTime = getDiff(timeHashingBegin);

    double vm2, rss2;
    process_mem_usage(vm2, rss2);

    double processedMemory = vm2 - vm;

    bool processedSuccessful = idealHashing->success;

    int idealHashingFalse = 0;
    auto averageSearchTimeBegin = chrono::system_clock::now();
    if (processedSuccessful) {
        for (auto i : v) {
            if (idealHashing->search(i.first) != i.second) {
                processedSuccessful = false;
            }
        }
    }
    double averageSearchTime = getDiff(averageSearchTimeBegin) / v.size();

    return result{processedSuccessful, hashingTime, averageSearchTime, processedMemory, idealHashing->attempts + 1};
}

template<typename T, typename C>
void measure(vector<pair<T, int>> (*generateFunction)(int),
             ofstream &out,
             vector<int> sizes = {50, 100, 500, 1000, 2000, 3000, 5000}) {

    int attemptsNum = 5;

    out << "p;size;hashing time(microseconds/10^-6 sec);average search time(microseconds/10^-6 sec);processed memory(KB);average number attempts\n";

    for (int p = 1; p <= 4; p += 1) {
        for (auto size : sizes) {
            double hashingTime = 0;
            double averageSearchTime = 0;
            double processedMemory = 0;
            double averageAttemptsNum = 0;
            bool processedSuccessful = true;

            vector<pair<T, int>> v = generateFunction(size);
            for (int attempt = 0; attempt < attemptsNum; attempt += 1) {
                result res = hashing<T, int, C>(v, p);
                hashingTime += res.hashingTime;
                averageSearchTime += res.averageSearchTime;
                processedMemory += res.processedMemory;
                processedSuccessful = processedSuccessful && res.processedSuccessful;
                averageAttemptsNum += res.numAttempts;
            }
            hashingTime /= attemptsNum;
            averageSearchTime /= attemptsNum;
            processedMemory /= attemptsNum;
            averageAttemptsNum /= attemptsNum;

            if (processedSuccessful) {
                out << p << ";" << size << ";" << hashingTime << ";"
                    << averageSearchTime << ";" << processedMemory << ";" << averageAttemptsNum << "\n";
            } else {
                cout << "Failed for p=" << p << " size=" << size << "\n";
            }
        }
    }

}