// O(n log n log w) without STL container

#include <cstdio>
#include <cstring>
#include <algorithm>

const int maxn = 1e5 + 3, maxlw = 30;

int n, pa[maxn], we[maxn], de[maxn];

struct edge {
	int v; edge *n;
} ep[maxn], *el[maxn];

int epl;
inline void addEdge(int u, int v, int w) {
	pa[v] = u;
	we[v] = w;
	el[u] = &(ep[epl++] = {v, el[u]});
}

long long ans;

struct node {
	int a, l; node *n;
};
struct blist_t {
	node p[maxlw], *r;
	int pl;
};
blist_t bl[maxn];

void copyBlist(blist_t &ori, blist_t &b, int w, int d) {
	b.pl = 1;
	b.r = &(b.p[0] = {w, d, NULL});
	node *prev = b.r;
	for(node *e = ori.r; e; e = e->n) {
		if(prev->a == (e->a & w)) {
			prev->l = e->l;
			continue;
		}
		prev = prev->n = b.p + b.pl++;
		*prev = {e->a & w, e->l, NULL};
	}
}

struct dm_t {
	int a[maxn * 3], l;
	inline void add(int k) {
		a[l++] = k - 1;
		a[l++] = k;
		a[l++] = k + 1;
	}
	inline void init() {
		std::sort(a, a + l);
		l = std::unique(a, a + l) - a;
	}
	inline int get(int k) {
		return std::lower_bound(a, a + l, k) - a;
	}
} dm;
int mp[maxn * 3];

void dfs(int u) {
	int prev_mp = -1;
	if(u > 1) {
		copyBlist(bl[pa[u]], bl[u], we[u], de[u]);

		int rk = dm.get(we[u]);
		prev_mp = mp[rk];
		mp[rk] = de[u];

		int l, r = de[u] + 1, pos;
		for(node *i = bl[u].r; i; i = i->n) {
			l = i->l;
			int irk = dm.get(i->a);
			if(mp[irk]) {
				pos = mp[irk];
				if(l <= pos && pos < r)
					ans += pos - l + 1;
			}
			r = l;
		}
	}
	for(edge *e = el[u]; e; e = e->n) {
		de[e->v] = de[u] + 1;
		dfs(e->v);
	}
	if(u > 1) {
		mp[dm.get(we[u])] = prev_mp;
	}
}

int main() {
	int t;
	scanf("%d", &t);
	while(t--) {
		epl = 0;
		memset(el, 0, sizeof(el));
		dm.l = 0;
		scanf("%d", &n);
		for(int i = 1, u, v, w; i < n; ++i) {
			scanf("%d%d%d", &v, &u, &w);
			addEdge(u, v, w);
			dm.add(w);
		}
		dm.init();
		ans = 0;
		dfs(1);
		printf("%lld\n", ans);
	}
	return 0;
}
