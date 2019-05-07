#include <bits/stdc++.h>

typedef long long ll;
const ll mo = 1e9 + 7;
const int maxn = 2e5 + 3, maxm = 20;
int a[maxn], f[maxn], n, m, q;

struct info {
    ll a[maxm];
    info operator*(const info &k) const {
        info res;
        memset(res.a, 0, sizeof(res.a));
        for(int i = 0; i < m; ++i) {
            for(int j = 0; j < m; ++j) {
                int tmp = i + j;
                tmp >= m ? tmp -= m : 0;
                res.a[tmp] = (res.a[tmp] + a[i] * k.a[j]) % mo;
            }
        }
        for(int i = 0; i < m; ++i) {
            res.a[i] = (res.a[i] + a[i] + k.a[i]) % mo;
        }
        return res;
    }
} bt[maxn];

int getF(int u) {
    if(f[u] == u) {
        return u;
    }
    int v = getF(f[u]);
    if(v == f[u])
        return v;
    bt[u] = bt[u] * bt[f[u]];
    f[u] = v;
    return v;
}

inline void uniF(int u, int v) {
    f[u] = v;
    bt[u].a[a[u - 1]] = 1;
}

struct que_t {
    int u, v;
} que[maxn];
int b[maxn], ans[maxn];

int main() {
    while(scanf("%d%d", &n, &m) != EOF) {
        for(int i = 0; i <= n + 1; ++i) {
            memset(bt[i].a, 0, sizeof(bt[i].a));
            f[i] = i;
        }
        for(int i = 0; i < n; ++i) {
            scanf("%d", a + i);
            a[i] %= m;
        }
        scanf("%d", &q);
        for(int i = 0; i < q; ++i) {
            scanf("%d%d", &que[i].u, &que[i].v);
            b[i] = i;
        }
        std::sort(b, b + q, [] (int i, int j) {
            return que[i].v < que[j].v;
        });
        for(int i = 1, j = 0; i <= n; ++i) {
            uniF(i, i + 1);
            for(; j < q && que[b[j]].v <= i; ++j) {
                getF(que[b[j]].u);
                ans[b[j]] = bt[que[b[j]].u].a[0] + 1;
            }
        }
        for(int i = 0; i < q; ++i)
            printf("%d\n", ans[i]);
    }
    return 0;
}
