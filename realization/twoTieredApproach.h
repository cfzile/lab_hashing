#pragma once

#include "idealHashing.h"

template<typename T, typename D, typename H>
class TwoTieredApproach {
public:
    vector<IdealHashing<T, D, H>> hashTable;
    H hashFunction;
    bool success = true;
    int hashTableSize;
    int attempts = 0, limit = 100;

    TwoTieredApproach(vector<pair<T, D>> data, int p = 3) {
        this->hashTableSize = p * data.size();
        hashFunction = H(hashTableSize);
        while (!hashing(data)) {
            attempts += 1;
            if (attempts > limit) {
                success = false;
                break;
            }
        }
    }

    bool hashing(vector<pair<T, D>> &data) {

        hashFunction.generateHashingFunction();
        hashTable.clear();

        vector<vector<pair<T, D>>> firstStep(hashTableSize);

        for (auto item : data) {
            auto itemHash = hashFunction.hashing(item.first);
            firstStep[itemHash].push_back(item);
        }

        for (int i = 0; i < firstStep.size(); i++) {
            hashTable.push_back(IdealHashing<T, D, H>(firstStep[i]));
            if (!hashTable.back().success) {
                return false;
            }
        }

        return true;
    }

    bool exist(T item) {
        return hashTable[hashFunction.hashing(item)].exist(item);
    }

    D search(T item) {
        return hashTable[hashFunction.hashing(item)].search(item);
    }
};