#pragma once
#include "general.h"


template<typename T, typename D, typename H>
class GraphHashing {
public:
    vector<vector<pair<int, int>>> edges;
    vector<pair<T, D>> saveData;
    vector<int> g;
    int attempts = 0, limits = 300, g_size = 587617;
    bool success = true;
    H f1, f2;

    explicit GraphHashing(vector<pair<T, D>> data, int g_size = 587617) {
        this->g_size = g_size;
        f1 = H(g_size);
        f2 = H(g_size);
        while (!hashing(data)) {
            attempts += 1;
            if (attempts > limits){
                success = false;
                break;
            }
        }
    }

    bool hashing(vector<pair<T, D>> & data) {

        g.resize(g_size);

        fill(g.begin(), g.end(), 0);

        saveData.clear();

        f1.generateHashingFunction();
        f2.generateHashingFunction();

        edges.resize(g_size);
        fill(edges.begin(), edges.end(), vector<pair<int, int>>());

        int num = 0;
        for (auto item : data) {
            auto hashF1 = f1.hashing(item.first);
            auto hashF2 = f2.hashing(item.first);
            edges[hashF1].push_back({ hashF2, num });
            edges[hashF2].push_back({ hashF1, num });
            num += 1;
        }

        vector<int> was(g_size, 0);

        bool successfulHashing = true;
        for (int i = 0; i < g_size; i++) {
            if (was[i] == 0) {
                successfulHashing = successfulHashing && findHValues(i, -1, was);
                if (!successfulHashing)
                    return false;
            }
        }

        for (auto item : data) {
            saveData.push_back(item);
        }

        return successfulHashing;
    }

    bool findHValues(int x, int p, vector<int> & was) {
        was[x] = 1;
        bool ans = true;
        for (auto item : edges[x]) {
            auto i = item.first;
            if (i == p)
                continue;
            g[i] = (item.second - g[x] + 2 * g.size()) % g.size();

            if (was[i] != 0 || i == x) {
                return false;
            } else {
                ans = ans && findHValues(i, x, was);
            }
        }
        was[x] = 2;
        return ans;
    }

    int getH(T item) {
        auto v = f1.hashing(item);
        auto u = f2.hashing(item);
        return (g[v] + g[u]) % g.size();
    }

    bool exist(T item) {
        auto h = getH(item);
        if (saveData[h].first == item) {
            return true;
        }
        return false;
    }

    D search(T item) {
        int h = getH(item);
        if (saveData[h].first != item) {
            cout << "no such element\n";
            throw exception();
        }
        return saveData[h].second;
    }
};