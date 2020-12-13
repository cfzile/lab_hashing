#pragma once
#include "general.h"
#include "hashFunctions.h"

template<typename T, typename D, typename H>
class IdealHashing {
public:
    vector<D> hashTable;
    bitset<25000000> hashTableUsed;
    H hashFunction;
    int attempts = 0;
    int limit = 1000;
    bool success = true;
    int hashTableSize;

    explicit IdealHashing(vector<pair<T, D>> data, int hashTableSize = 1) {
        this->hashTableSize = hashTableSize;
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
        hashTableUsed = 0;

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

template<typename T, typename D, typename H>
class IdealHashingAdvanced {
public:
    vector<IdealHashing<T, D, H>> hashTable;
    H hashFunction;
    bool success = true;
    int p;

    explicit IdealHashingAdvanced(vector<pair<T, D>> data, int p = 1) {
        this->p = p;
        hashFunction = H(p * data.size());
        hashing(data);
    }

    void hashing(vector<pair<T, D>> & data) {

        hashFunction.generateHashingFunction();
        hashTable.clear();

        vector<vector<pair<T, D>>> firstStep(p * data.size());

        for (auto item : data){
            auto itemHash = hashFunction.hashing(item.first);
            firstStep[itemHash].push_back(item);
        }

        for (int i = 0; i < firstStep.size(); i++) {
            hashTable.push_back(IdealHashing<T, D, H>(firstStep[i]));
            if (!hashTable.back().success) {
                success = false;
            }
        }

    }

    bool exist(T item) {
        return hashTable[hashFunction.hashing(item)].exist(item);
    }

    D search(T item) {
        return hashTable[hashFunction.hashing(item)].search(item);
    }
};
