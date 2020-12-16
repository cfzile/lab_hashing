#pragma once

#include "general.h"
#include "hashFunctions.h"

template<typename T, typename D, typename H, int n = 1000000>
class IdealHashing {
public:
    vector<D> hashTable;
    bitset<n> hashTableUsed;
    H hashFunction;
    int attempts = 0, limit = 200, hashTableSize;
    bool success = true;

    IdealHashing(vector<pair<T, D>> data, int p = 1) {
        hashTableSize = p * (int)data.size() * (int)data.size();
        hashFunction = H(hashTableSize);
        hashTable = vector<D>(hashTableSize);
        while (!hashing(data)) {
            attempts += 1;
            if (attempts > limit) {
                success = false;
                break;
            }
        }
    }

    bool hashing(vector<pair<T, D>> &data) {
        hashTableUsed = 0;
        hashFunction.generateHashingFunction();

        for (auto item : data) {
            auto itemHash = hashFunction.hashing(item.first);
            if (hashTableUsed[itemHash]) {
                return false;
            }
            hashTableUsed[itemHash] = 1;
            hashTable[itemHash] = item.second;
        }
        return true;
    }

    bool exist(T item) {
        return hashTableUsed[hashFunction.hashing(item)];
    }

    D search(T item) {
        return hashTable[hashFunction.hashing(item)];
    }
};
