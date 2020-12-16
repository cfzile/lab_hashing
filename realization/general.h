#pragma once

#include <bits/stdc++.h>
#include <sys/resource.h>
#include <unistd.h>

#define int long long
#define double long double

int inf = 20000000;

using namespace std;

vector<int> primes = {4294967295, 587617, 96873151, 49782769, 25556317, 32287093, 84180703, 29502881, 60573089, 41091107, 19289551,

                      28036597};
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
    vm_usage = vsize / 1024.; // Mбайт
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
void measure(vector<pair<T, int>> (*generateFunction)(long long),
             ofstream &out,
             vector<int> sizes = {50, 100, 500, 1000, 2000}) {

    int attemptsNum = 5;

    out << "p;size;hashing time;average search time;processed memory;number attempts\n";

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