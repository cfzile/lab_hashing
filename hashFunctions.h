#pragma once
#include "general.h"

template<typename T>
class HashFunction {
public:
    void generateHashingFunction();
    int hashing(T);
};

// universal hashFunctions

template<typename T>
class HashFunctionType1 : public HashFunction<T> {
public:
    vector<int> a;
    int b, p, n;

    HashFunctionType1() {}

    explicit HashFunctionType1(int n) {
        this->n = n;
        generateHashingFunction();
    }

    void generateHashingFunction(){
        p = primes[generateInteger(0, primes.size() - 1)];
        a = generateVectorInteger(20, 1, p - 1);
        b = generateInteger(0, p - 1);
    }

    int hashing(int item) {
        return ((a[0] * item + b) % p) % n;
    }

    int hashing(string item) {
        int hash = 5381;
        int num = 0;
//        int A = a[0];
        for (auto i : item) {
            hash = ((hash * (a[i] % p)) + (int)i) % p;
            num += 1;
//            A = (1 * a[0]) % n;
        }
//        cout << item << " " << hash << "\n";
        return hash % n;
//        return ((hash * a[1] + b) % p) % n;
    }
};

//  hashFunction for graphHashing

template<typename T>
class HashFunctionType2 : public HashFunction<T> {
public:
    vector<int> table;
    int n;

    HashFunctionType2(){}

    explicit HashFunctionType2(int n) {
        this->n = n;
        generateHashingFunction();
    }

    void generateHashingFunction() {
        table.clear();
        for (int i = 0; i < 1000; i++){
            table.push_back(generateInteger(0, inf));
        }
    }

    int hashing(string item) {
        auto hash = 0;
        int num = 0;
        for (auto i : item) {
            int g = (int) i;
            hash = (hash + g * table[num]) % n;
            num += 1;
        }
        return hash;
    }

    int hashing(int item) {
        auto hash = 0;
        int num = 0;
        while (item) {
            hash = (hash + (item % 2) * table[num]) % n;
            item /= 2;
            num += 1;
        }
        return hash;
    }
};


template<typename T>
class HashFunctionType3 : public HashFunction<T> {
public:
    vector<vector<int>> table;
    int n;

    HashFunctionType3(){}

    explicit HashFunctionType3(int n) {
        this->n = n;
        generateHashingFunction();
    }

    void generateHashingFunction(){
        table.clear();
        for (int i = 0; i < 100; i++){
            table.emplace_back();
            for (int j = 0; j < 500; j++) {
                table[i].push_back(generateInteger(0, n));
            }
        }
    }

    int hashing(string item) {
        auto hash = 0;
        int num = 0;
        for (auto i : item) {
            hash = (hash + table[num][i]) % n;
            num += 1;
        }
        return hash;
    }

    int hashing(int item) {
        auto hash = 0;
        int num = 0;
        while (item) {
            hash = (hash + table[(item % 2)][num]) % n;
            item /= 2;
            num += 1;
        }
        return hash;
    }
};
