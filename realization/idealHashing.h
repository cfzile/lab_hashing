#pragma once

#include "general.h"
#include "hashFunctions.h"

template<typename T, typename D, typename H>
class IdealHashing {
public:
    vector<D> hashTable;
    vector<int> hashTableUsed;
    H hashFunction;
    int attempts = 0, limit = 200, hashTableSize;
    int t = 1;
    bool success = true;

    IdealHashing(vector<pair<T, D>> data, int p = 1) {
        hashTableSize = p * (int)data.size() * (int)data.size();
        hashFunction = H(hashTableSize);
        hashTable = vector<D>(hashTableSize);
        hashTableUsed = vector<int>(hashTableSize);
        while (!hashing(data)) {
            attempts += 1;
            if (attempts > limit) {
                success = false;
                break;
            }
        }
    }

    bool hashing(vector<pair<T, D>> &data) {
        t += 1;
        hashFunction.generateHashingFunction();

        for (auto item : data) {
            auto itemHash = hashFunction.hashing(item.first);
            if (hashTableUsed[itemHash] == t) {
                return false;
            }
            hashTableUsed[itemHash] = t;
            hashTable[itemHash] = item.second;
        }
        return true;
    }

    bool exist(T item) {
        return hashTableUsed[hashFunction.hashing(item)] == t;
    }

    D search(T item) {
        return hashTable[hashFunction.hashing(item)];
    }
};
