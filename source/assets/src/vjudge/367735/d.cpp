#include <bits/stdc++.h>

const int maxn = 2e5 + 3;

int n;
std::vector<int> ans;

void rootleft(int r, int u, int v);
void rightleft(int r, int u, int v);
void rightroot(int r, int u, int v);

void rootleft(int r, int u, int v) {
    ans.push_back(r);
    if(u == r)
        return;
    rootleft(r * 2 + 1, u / r / 2 * (r * 2 + 1), v);
    rightleft(r * 2, u, u / r / 2 * (r * 2 + 1) - 1);
}

void rightleft(int r, int u, int v) {
    if(u != r)
        rightroot(r * 2 + 1, u / r / 2 * (r * 2 + 1), v);
    ans.push_back(r);
    if(u != r)
        rootleft(r * 2, u, u / r / 2 * (r * 2 + 1) - 1);
}

void rightroot(int r, int u, int v) {
    if(u != r) {
        rightleft(r * 2 + 1, u / r / 2 * (r * 2 + 1), v);
        rightroot(r * 2, u, u / r / 2 * (r * 2 + 1) - 1);
    }
    ans.push_back(r);
}

std::vector<int> lroots, rroots;

void rl(int r) {
    int u = r, v = r;
    while(v * 2 + 1 <= n) {
        u = u * 2;
        v = v * 2 + 1;
    }
    rightleft(r, u, v);
}

void solve() {
    ans.clear();
    scanf("%d", &n);
    lroots.clear();
    rroots.clear();
    int lo = 1;
    while(lo * 2 <= n)
        lo *= 2;
    for(int i = 1, j = lo / 2; i <= n; j /= 2) {
        ans.push_back(i);
        if(i == n)
            break;
        if(n & j) {
            if(i * 2 <= n)
                lroots.push_back(i * 2);
            i = i * 2 + 1;
        } else {
            if(i * 2 + 1 <= n)
                rroots.push_back(i * 2 + 1);
            i = i * 2;
        }
    }
    for(int i = lroots.size() - 1; i >= 0; --i)
        rl(lroots[i]);
    for(int i = 0; i < rroots.size(); ++i)
        rl(rroots[i]);
    for(int i = 0; i < ans.size(); ++i)
        printf("%d%c", ans[i], " \n"[i == ans.size() - 1]);
}

int main() {
    int t;
    scanf("%d", &t);
    while(t--)
        solve();
    return 0;
}
