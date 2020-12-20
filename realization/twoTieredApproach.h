#pragma once

#include "idealHashing.h"

template<typename T, typename D, typename H>
class TwoTieredApproach {
public:
    vector<IdealHashing<T, D, H>> hashTable;
    H hashFunction;
    bool success = true;
    int hashTableSize;
    int attempts = 1, p;

    TwoTieredApproach(vector<pair<T, D>> data, int p = 1) {
        this->hashTableSize = data.size();
        this->p = p;
        hashFunction = H(hashTableSize);
        success = hashing(data);
    }

    bool hashing(vector<pair<T, D>> &data) {

        vector<vector<pair<T, D>>> firstStep(hashTableSize);

        int collisions = 0;
        int limit = 100;

        do {
            collisions = 0;

            hashFunction.generateHashingFunction();

            for (auto & i : firstStep) {
                i.clear();
            }

            for (auto item : data) {
                auto itemHash = hashFunction.hashing(item.first);
                firstStep[itemHash].push_back(item);
            }

            for (int i = 0; i < firstStep.size(); i++) {
                int size = firstStep[i].size();
                collisions += size * size;
            }
        } while (collisions > p * data.size() && limit--);

        if (limit == 0) {
            return false;
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