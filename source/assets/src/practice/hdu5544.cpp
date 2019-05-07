#include <bits/stdc++.h>

typedef long long unsigned ll;

const int maxn = 1e5 + 3;

int n, m;

struct edge {
    int v;
    ll w;
    edge *n;
} ep[2 * maxn], *el[maxn];

int epl;

void addEdge(int u, int v, const ll &w) {
    el[u] = &(ep[epl++] = {v, w, el[u]});
    el[v] = &(ep[epl++] = {u, w, el[v]});
}

struct base_t {
    int r;
    ll a[64];

    void clear() {
        r = 0;
        memset(a, 0, sizeof(a));
    }

    void add(ll w) {
        for(int i = 0; i < 64; ++i) {
            ll v = 1ull << (63 - i);
            if(w & v) {
                if(a[i])
                    w ^= a[i];
                else {
                    ++r;
                    a[i] = w;
                    break;
                }
            }
        }
    }
} base;

ll a[maxn];
bool vis[maxn];

ll calc() {
    ll ans = 0;
    for(int i = 0; i < 64; ++i) {
        ll v = 1ull << (63 - i);
        if(!(ans & v) && base.a[i])
            ans ^= base.a[i];
    }
    return ans;
}

void dfs(int u) {
    vis[u] = true;
    for(edge *e = el[u]; e; e = e->n) {
        if(vis[e->v]) {
            base.add(a[u] ^ a[e->v] ^ e->w);
        } else {
            a[e->v] = a[u] ^ e->w;
            dfs(e->v);
        }
    }
}

ll solve() {
    scanf("%d%d", &n, &m);
    base.clear();
    epl = 0;
    memset(el, 0, sizeof(edge *) * (n + 1));
    memset(vis, 0, sizeof(bool) * (n + 1));
    for(int i = 0; i < m; ++i) {
        int u, v;
        ll w;
        scanf("%d%d%lld", &u, &v, &w);
        addEdge(u, v, w);
    }
    a[1] = 0;
    dfs(1);
    return calc();
}

int main() {
    int t;
    scanf("%d", &t);
    for(int i = 1; i <= t; ++i) {
        printf("Case #%d: %llu\n", i, solve());
    }
    return 0;
}
