#include <bits/stdc++.h>

typedef long long ll;
const int maxn = 2e5 + 3;

int n, s[maxn], p[maxn];
ll a[maxn];
bool b[maxn];

void solve() {
    scanf("%d", &n);
    for(int i = 0; i < n; ++i)
        scanf("%lld", a + i);
    for(int i = 0; i < n; ++i) {
        b[i] = i - 1 >= 0 && a[i] > a[i - 1] && i + 1 < n && a[i] > a[i + 1];
        p[i] = (i ? p[i - 1] : 0) + b[i];
    }
    for(int i = n - 1; i >= 0; --i) {
        s[i] = (i + 1 < n ? s[i + 1] : 0) + b[i];
    }
    int ans = p[n - 1];
    for(int i = 0; i < n; ++i) {
        int l = i - 2 >= 0 ? p[i - 2] : 0;
        int r = i + 2 < n ? s[i + 2] : 0;
        int w = 0;
        if(i - 2 >= 0 && i + 1 < n && a[i - 1] > a[i - 2] && a[i - 1] > a[i + 1])
            w += 1;
        if(i + 2 < n && i - 1 >= 0 && a[i + 1] > a[i + 2] && a[i + 1] > a[i - 1])
            w += 1;
        ans = std::min(ans, l + r + w);
    }
    printf("%d\n", ans);
}

int main() {
    int t;
    scanf("%d", &t);
    while(t--)
        solve();
    return 0;
}
