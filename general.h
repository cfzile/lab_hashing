#pragma once
#include <bits/stdc++.h>
#include <sys/resource.h>
#include <unistd.h>

#define int long long
#define double long double

int inf = 2000000000;

using namespace std;

vector<int> primes = {587617,96873151,49782769,25556317,32287093,84180703,29502881,60573089,41091107,19289551,28036597};
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
            int string_size = generateInteger(0, 20) + 1;
            str = "";
            for (int j = 0; j < string_size; j++) {
                str += (char) generateInteger(65, 127);
            }
        }while(keys.find(str) != keys.end());
        result.emplace_back(str, generateInteger(0, inf));
        keys.insert(str);
    }
    return result;
}

void process_mem_usage(double& vm_usage, double& resident_set)
{
    vm_usage     = 0.0;
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
    vm_usage = vsize / 1024.0;
    resident_set = rss * page_size_kb;
}
