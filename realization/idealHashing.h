#pragma once
#include "general.h"
#include "hashFunctions.h"

template<typename T, typename D, typename H>
class IdealHashing {
public:
    vector<D> hashTable;
    vector<bool> hashTableUsed;
    H hashFunction;
    int attempts = 0;
    int limit = 200;
    bool success = true;
    int hashTableSize;

    IdealHashing(vector<pair<T, D>> data, int p = 1) {
        this->hashTableSize = p * data.size() * data.size();
        hashFunction = H(hashTableSize);
        hashTable.resize(hashTableSize);
        while (!hashing(data)) {
            attempts += 1;
            if (attempts > limit){
                success = false;
                break;
            }
        }
    }

    bool hashing(vector<pair<T, D>> & data) {
        hashFunction.generateHashingFunction();
        hashTableUsed.clear();
        hashTableUsed.resize(hashTableSize);

        for (auto item : data) {
            auto itemHash = hashFunction.hashing(item.first);
            if (hashTableUsed[itemHash]) {
                return false;
            }
            hashTableUsed[itemHash] = true;
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
