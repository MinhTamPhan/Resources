#pragma once
#include <bits/stdc++.h>
using namespace std;

using T = long double; const T EPS = 1e-9;

bool lt(T a, T b) { return a + EPS < b; }
bool le(T a, T b) { return !lt(b, a); }
bool gt(T a, T b) { return lt(b, a); }
bool ge(T a, T b) { return !lt(a, b); }
bool eq(T a, T b) { return !lt(a, b) && !lt(b, a); }
bool ne(T a, T b) { return lt(a, b) || lt(b, a); }

struct EpsPairCmp {
    bool operator () (const pair<T, T> &a, const pair<T, T> &b) const {
        return eq(a.first, b.first) ? lt(a.second, b.second) : lt(a.first, b.first);
    }
};

// Adding and removing intervals from a set
// Time Complexity:
//   addInterval, removeInterval: O(log N) amortized
// Memory Complexity: O(N)
struct IntervalUnion : public set<pair<T, T>, EpsPairCmp> {
    set<pair<T, T>, EpsPairCmp>::iterator addInterval(T L, T R) { // adds the interval [L, R)
        if (eq(L, R)) return end();
        auto it = lower_bound(make_pair(L, R)), before = it;
        while (it != end() && le(it->first, R)) { R = max(R, it->second); before = it = erase(it); }
        if (it != begin() && ge((--it)->second, L)) { L = min(L, it->first); R = max(R, it->second); erase(it); }
        return emplace_hint(before, L, R);
    }
    void removeInterval(T L, T R) { // removes the interval [L, R)
        if (eq(L, R)) return;
        auto it = addInterval(L, R);
        auto r2 = it->second;
        if (eq(it->first, L)) erase(it);
        else (T&) it->second = L;
        if (ne(R, r2)) emplace(R, r2);
    }
};

// Given a set of intervals, combine them into disjoint intervals of the form [L, R)
// Returns an iterator to right after the last disjoint interval
// Time Complexity: O(N)
// Memory Complexity: O(1)
template <class It> It intervalUnion(It st, It en) {
    assert(is_sorted(st, en, EpsPairCmp()));
    It cur = st;
    for (It l = st, r; l < en; l = r, cur++) {
        *cur = *l;
        for (r = l + 1; r < en && le(r->first, cur->second); r++) cur->second = max(cur->second, r->second);
    }
    return cur;
}
