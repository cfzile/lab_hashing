#pragma once

#include "general.h"

template<typename T>
class HashFunction {
public:
    void generateHashingFunction();

    long long hashing(T);
};

// universal hashFunctions

template<typename T>
class HashFunctionType1 : public HashFunction<T> {
public:
    long long a, b, p, n;

    explicit HashFunctionType1() {}

    explicit HashFunctionType1(long long n) {
        this->n = n;
        generateHashingFunction();
    }

    void generateHashingFunction() {
        p = primes[generateInteger(0, primes.size() - 1)];
        a = generateInteger(1, p - 1);
        b = generateInteger(0, p - 1);
    }

    long long hashing(long long item) {
        return ((a * item + b) % p) % n;
    }

    long long hashing(string item) {
        long long hash = 5381;
        for (auto i : item) {
            long long g = (long long) i + 300;
            hash = (hash * a + g) % p;
        }
        return hash % n;
    }
};

//  hashFunction for graphHashing

template<typename T>
class HashFunctionType2 : public HashFunction<T> {
public:
    vector<long long> table;
    long long n;

    HashFunctionType2() {}

    explicit HashFunctionType2(long long n) {
        this->n = n;
        generateHashingFunction();
    }

    void generateHashingFunction() {
        table.clear();
        for (long long i = 0; i < 1000; i++) {
            table.push_back(generateInteger(0, inf));
        }
    }

    int hashing(string item) {
        auto hash = 0;
        long long num = 0;
        for (auto i : item) {
            long long g = (long long) i + 300;
            hash = (hash + g * table[num]) % n;
            num += 1;
        }
        return hash;
    }

    int hashing(long long item) {
        auto hash = 0;
        long long num = 0;
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
    vector<vector<long long>> table;
    long long n;

    HashFunctionType3() {}

    explicit HashFunctionType3(long long n) {
        this->n = n;
        generateHashingFunction();
    }

    void generateHashingFunction() {
        table.clear();
        for (int i = 0; i < 100; i++) {
            table.emplace_back();
            for (int j = 0; j < 500; j++) {
                table[i].push_back(generateInteger(0, n));
            }
        }
    }

    int hashing(string item) {
        auto hash = 0;
        long long num = 0;
        for (auto i : item) {
            hash = (hash + table[num][i]) % n;
            num += 1;
        }
        return hash;
    }

    int hashing(long long item) {
        auto hash = 0;
        long long num = 0;
        while (item) {
            hash = (hash + table[(item % 2)][num]) % n;
            item /= 2;
            num += 1;
        }
        return hash;
    }
};
