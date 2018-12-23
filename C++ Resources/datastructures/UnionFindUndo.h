#pragma once
#include <bits/stdc++.h>
using namespace std;

// Union Find / Disjoint Sets supporting undos
// Time Complexity:
//   init: O(N)
//   find, join, connected, getSize: O(log N)
//   undo: O(1)
// Memory Complexity: O(N)
template <const int MAXN> struct UnionFindUndo {
    int UF[MAXN], cnt; stack<pair<pair<int, int>, int>> history;
    void init(int N) { cnt = N; fill(UF, UF + MAXN, -1); }
    int find(int v) { while (UF[v] >= 0) v = UF[v]; return v; }
    bool join(int v, int w) {
        v = find(v); w = find(w);
        if (v == w) return false;
        if (UF[v] > UF[w]) swap(v, w);
        history.push({{v, w}, UF[w]}); UF[v] += UF[w]; UF[w] = v; cnt--;
        return true;
    }
    void undo() {
        int v = history.top().first.first, w = history.top().first.second, ufw = history.top().second;
        history.pop(); UF[w] = ufw; UF[v] -= UF[w]; cnt++;
    }
    bool connected(int v, int w) { return find(v) == find(w); }
    int getSize(int v) { return -UF[find(v)]; }
};