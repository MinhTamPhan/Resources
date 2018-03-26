#ifndef DATASTRUCTURES_TREES_DYNAMICSEGMENTTREE_H_
#define DATASTRUCTURES_TREES_DYNAMICSEGMENTTREE_H_

#include <bits/stdc++.h>
using namespace std;

struct DynamicSegmentTree {
private:
    static const int vdef = 0, qdef = 0;

    static int merge(int l, int r) {
        return l + r;
    }

    static int apply(int x, int v) {
        return x + v;
    }

    struct Node {
        Node *left = nullptr, *right = nullptr;
        int val = DynamicSegmentTree::vdef;
    };

    Node *root;
    int N;

    void update(Node *cur, int cL, int cR, int ind, int val) {
        if (cL > ind || cR < ind) return;
        if (cL >= ind && cR <= ind) {
           cur->val = apply(cur->val, val);
           return;
        }
        int m = cL + (cR - cL) / 2;
        if (cur->left == nullptr) cur->left = new Node();
        update(cur->left, cL, m, ind, val);
        if (cur->right == nullptr) cur->right = new Node();
        update(cur->right, m + 1, cR, ind, val);
        cur->val = merge(cur->left->val, cur->right->val);
    }

    int query(Node *cur, int cL, int cR, int l, int r) {
        if (cur == nullptr || cL > r || cR < l) return qdef;
        if (cL == cR) return cur->val;
        int m = cL + (cR - cL) / 2;
        int left = query(cur->left, cL, m, l, r);
        int right = query(cur->right, m + 1, cR, l, r);
        return merge(left, right);
    }

public:
    DynamicSegmentTree(int size) {
        root = new Node();
        N = size;
    }

    void update(int ind, int val) {
        update(root, 1, N, ind, val);
    }

    int query(int l, int r) {
        return query(root, 1, N, l, r);
    }

    int size() {
        return N;
    }
};

#endif /* DATASTRUCTURES_TREES_DYNAMICSEGMENTTREE_H_ */
