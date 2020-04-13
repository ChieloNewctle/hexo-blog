#include <bits/stdc++.h>

typedef long long ll;
const int maxn = 2e5 + 3;

int n, a[maxn], c[maxn], b[maxn];
std::map<int, int> s, as;
std::set<std::pair<int, int> > sa;

void deca(int k) {
    int j = as[k];
    as[k] -= 1;
    sa.erase(sa.find(std::make_pair(j, k)));
    if(j - 1 > 0)
        sa.insert(std::make_pair(j - 1, k));
}

void dec(int k) {
    int j = s[k];
    s[k] -= 1;
    if(j - 1 == 0) {
        s.erase(k);
    }
    deca(k);
}

void solve() {
    scanf("%d", &n);
    for(int i = 0; i < n; ++i)
        scanf("%d", a + i);
    memset(c, 0, sizeof(c[0]) * (n + 1));
    s.clear();
    as.clear();
    sa.clear();
    for(int i = 0; i < n; ++i)
        c[a[i]] += 1;
    for(int i = 1; i <= n; ++i)
        if(c[i] * 2 > n) {
            puts("Impossible");
            return;
        }
    for(int i = 1; i <= n; ++i)
    if(c[i]) {
        s[i] = c[i];
        as[i] = c[i] * 2;
        sa.insert(std::make_pair(c[i] * 2, i));
    }
    for(int i = 0; i < n; ++i) {
        deca(a[i]);
        std::set<std::pair<int, int> >::iterator jt = sa.end();
        --jt;
        if(jt->first > (n - i - 1)) {
            b[i] = jt->second;
            dec(jt->second);
            continue;
        }
        std::map<int, int>::iterator it = s.begin();
        if(it->first == a[i])
            ++it;
        b[i] = it->first;
        dec(it->first);
    }
    for(int i = 0; i < n; ++i)
        printf("%d%c", b[i], " \n"[i == n - 1]);
}

int main() {
    int t;
    scanf("%d", &t);
    while(t--)
        solve();
    return 0;
}
