#pragma once
#include <bits/stdc++.h>
using namespace std;

// Top down segment tree supporting point updates and range queries,
//   with a lazy template argument to enable range updates
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// Template Arguments:
//   LAZY: boolean to indicate whether or not range updates are enabled
//   Combine: struct to combine data and lazy values
//     Fields:
//       Data: typedef/using for the data type
//       Lazy: typedef/using for the lazy type
//       qdef: the query default value of type Data
//       ldef (only required if LAZY is true): the lazy default value of
//         type Lazy
//     Functions:
//       merge(l, r): returns the values l of type Data merged with
//         r of type Data, must be associative
//       applyLazy(l, r): returns the value r of type Lazy applied to
//         l of type Data, must be associative
//       getSegmentVal(v, k) (only required if LAZY is true): returns the lazy
//         value v when applied over a segment of length k
//       mergeLazy(l, r) (only required if LAZY is true): returns the values
//         l of type Lazy merged with r of type Lazy, must be associative
//     Sample Struct: Combine supports point increments and range max queries,
//         while Combine2 supports range assignments and range sum queries
//       struct Combine {
//         using Data = int;
//         using Lazy = int;
//         const Data qdef = numeric_limits<int>::min();
//         Data merge(const Data &l, const Data &r) const { return max(l, r); }
//         Data applyLazy(const Data &l, const Lazy &r) const { return l + r; }
//       };
//       struct Combine2 {
//         using Data = int;
//         using Lazy = int;
//         const Data qdef = 0;
//         const Lazy ldef = numeric_limits<int>::min();
//         Data merge(const Data &l, const Data &r) const { return l + r; }
//         Data applyLazy(const Data &l, const Lazy &r) const { return r; }
//         Lazy getSegmentVal(const Lazy &v, int k) const { return v * k; }
//         Lazy mergeLazy(const Lazy &l, const Lazy &r) const { return r; }
//       };
// Constructor Arguments:
//   N: the size of the array
//   f: a generating function that returns the ith element on the ith call
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
//   vdef: the default value to fill the array with
// Functions:
//   update(i, v): updates the index i with the lazy value v
//   query(l, r): queries the range [l, r] and returns the aggregate value
// In practice, has a small constant, not quite as fast as fenwick trees
//   or bottom up non lazy segment trees,
//   and similar performance as bottom up lazy segment trees
// Time Complexity:
//   constructor: O(N)
//   update, query: O(log N)
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/ds3 (LAZY = false)
//   https://dmoj.ca/problem/dmpg17g2 (LAZY = false)
//   https://dmoj.ca/problem/coci17c1p5 (LAZY = false)
//   https://dmoj.ca/problem/cco20p5 (LAZY = false)
//   https://www.spoj.com/problems/BRCKTS/ (LAZY = false)
//   https://judge.yosupo.jp/problem/point_set_range_composite (LAZY = false)
//   https://dmoj.ca/problem/dmopc17c4p6 (LAZY = true)
//   https://dmoj.ca/problem/lazy (LAZY = true)
//   https://mcpt.ca/problem/seq3 (LAZY = true)
//   https://judge.yosupo.jp/problem/range_affine_range_sum (LAZY = true)
template <const bool LAZY, class Combine> struct SegmentTreeTopDown {
#define lazy_def template <const bool _ = LAZY> typename enable_if<_>::type
#define agg_def template <const bool _ = LAZY> typename enable_if<!_>::type
  using Data = typename Combine::Data; using Lazy = typename Combine::Lazy;
  template <const bool _, const int __ = 0> struct Node {
    Data val; Node() : val(Combine().qdef) {}
  };
  template <const int __> struct Node<true, __> {
    Data val; Lazy lz; Node() : val(Combine().qdef), lz(Combine().ldef) {}
  };
  Combine C; int N; vector<Node<LAZY>> TR;
  lazy_def apply(int x, int tl, int tr, const Lazy &v) {
    TR[x].val = C.applyLazy(TR[x].val, C.getSegmentVal(v, tr - tl + 1));
    TR[x].lz = C.mergeLazy(TR[x].lz, v);
  }
  agg_def apply(int x, int, int, const Lazy &v) {
    TR[x].val = C.applyLazy(TR[x].val, v);
  }
  lazy_def propagate(int x, int tl, int tr) {
    if (TR[x].lz != C.ldef) {
      int m = tl + (tr - tl) / 2; apply(x * 2, tl, m, TR[x].lz);
      apply(x * 2 + 1, m + 1, tr, TR[x].lz); TR[x].lz = C.ldef;
    }
  }
  agg_def propagate(int, int, int) {}
  template <class F> void build(int x, int tl, int tr, F &f) {
    if (tl == tr) { TR[x].val = f(); return; }
    int m = tl + (tr - tl) / 2;
    build(x * 2, tl, m, f); build(x * 2 + 1, m + 1, tr, f);
    TR[x].val = C.merge(TR[x * 2].val, TR[x * 2 + 1].val);
  }
  void update(int x, int tl, int tr, int l, int r, const Lazy &v) {
    if (l <= tl && tr <= r) { apply(x, tl, tr, v); return; }
    propagate(x, tl, tr); int m = tl + (tr - tl) / 2;
    if (tl <= r && l <= m) update(x * 2, tl, m, l, r, v);
    if (m + 1 <= r && l <= tr) update(x * 2 + 1, m + 1, tr, l, r, v);
    TR[x].val = C.merge(TR[x * 2].val, TR[x * 2 + 1].val);
  }
  Data query(int x, int tl, int tr, int l, int r) {
    if (r < tl || tr < l) return C.qdef;
    if (l <= tl && tr <= r) return TR[x].val;
    propagate(x, tl, tr); int m = tl + (tr - tl) / 2;
    return C.merge(query(x * 2, tl, m, l, r),
                   query(x * 2 + 1, m + 1, tr, l, r));
  }
  template <class F> SegmentTreeTopDown(int N, F f)
      : N(N), TR(N == 0 ? 0 : 1 << __lg(N * 4 - 1)) {
    if (N > 0) { build(1, 0, N - 1, f); }
  }
  template <class It> SegmentTreeTopDown(It st, It en)
      : SegmentTreeTopDown(en - st, [&] { return *st++; }) {}
  SegmentTreeTopDown(int N, const Data &vdef)
      : SegmentTreeTopDown(N, [&] { return vdef; }) {}
  lazy_def update(int l, int r, const Lazy &v) {
    update(1, 0, N - 1, l, r, v);
  }
  void update(int i, const Lazy &v) { update(1, 0, N - 1, i, i, v); }
  Data query(int l, int r) { return query(1, 0, N - 1, l, r); }
#undef lazy_def
#undef agg_def
};
