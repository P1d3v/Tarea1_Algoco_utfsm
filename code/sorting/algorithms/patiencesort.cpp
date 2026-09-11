#include "patiencesort.h"
#include <queue>
#include <algorithm>
using namespace std;
// https://en.wikipedia.org/wiki/Patience_sorting
void patienceSort(vector<int>& arr) {
    if (arr.size() < 2) return;

    vector<vector<int>> piles;

    for (int card : arr) {
        auto it = lower_bound(
            piles.begin(), piles.end(), card,
            [](const vector<int>& pile, int value) {
                return pile.back() < value;
            });

        if (it == piles.end()) {
            piles.push_back({card});
        } else {
            it->push_back(card);
        }
    }

    using Entry = pair<int, int>;
    priority_queue<Entry, vector<Entry>, greater<Entry>> minHeap;

    for (int i = 0; i < static_cast<int>(piles.size()); ++i) {
        minHeap.push({piles[i].back(), i});
    }

    int pos = 0;
    while (!minHeap.empty()) {
        Entry top = minHeap.top();
        int value = top.first;
        int pileIdx = top.second;
        minHeap.pop();

        arr[pos++] = value;
        piles[pileIdx].pop_back();

        if (!piles[pileIdx].empty()) {
            minHeap.push({piles[pileIdx].back(), pileIdx});
        }
    }
}