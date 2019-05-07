#include <bits/stdc++.h>

typedef long long ll;
const ll mo = 1e9 + 7;
const int maxn = 2e5 + 3, maxm = 20;

int a[maxn], f[maxn], n, m, q;

struct info {
    ll a[maxm];

    void clear() {
        memset(a, 0, sizeof(ll) * m);
    }

    ll operator*(const info &k) const {
        ll res = 0;
        for(int i = 0; i < m; ++i)
            res = (res + a[i] * k.a[(m - i) % m]) % mo;
        return res;
    }

    void offset(int k) {
        ll b[maxm];
        memcpy(b, a, sizeof(ll) * m);
        for(int i = 0; i < m; ++i) {
            a[i] = (a[i] + b[(i - k + m) % m]) % mo;
        }
    }
} bt[maxn];

struct que_t {
    int u, v;
} que[maxn];

int b[maxn], ans[maxn];

void dac(int l, int r, int ql, int qr) {
    if(ql >= qr)
        return;
    if(r - l <= 1) {
        for(int i = ql; i < qr; ++i)
            ans[b[i]] = (a[l] == 0 ? 2 : 1);
        return;
    }

    int mid = (l + r) / 2;
    
    int qar = std::partition(b + ql, b + qr, [mid] (int i) {
        return que[i].v < mid;
    }) - b;
    int qbl = std::partition(b + qar, b + qr, [mid] (int i) {
        return que[i].u < mid;
    }) - b;
    
    dac(l, mid, ql, qar);
    dac(mid, r, qbl, qr);
    
    bt[mid].clear();
    bt[mid].a[0] += 1;
    bt[mid].a[a[mid]] += 1;
    for(int i = mid + 1; i < r; ++i) {
        bt[i] = bt[i - 1];
        bt[i].offset(a[i]);
    }
    bt[mid - 1].clear();
    bt[mid - 1].a[0] += 1;
    bt[mid - 1].a[a[mid - 1]] += 1;
    for(int i = mid - 2; i >= l; --i) {
        bt[i] = bt[i + 1];
        bt[i].offset(a[i]);
    }

    for(int i = qar; i < qbl; ++i)
        ans[b[i]] = bt[que[b[i]].u] * bt[que[b[i]].v];
}

int main() {
    scanf("%d%d", &n, &m);
    for(int i = 1; i <= n; ++i) {
        scanf("%d", a + i);
        a[i] %= m;
    }
    scanf("%d", &q);
    for(int i = 0; i < q; ++i) {
        scanf("%d%d", &que[i].u, &que[i].v);
        b[i] = i;
    }

    dac(1, n + 1, 0, q);
    
    for(int i = 0; i < q; ++i)
        printf("%d\n", ans[i]);
    return 0;
}
