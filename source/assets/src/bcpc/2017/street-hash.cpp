#include <cstdio>
#include <cstring>
#include <list>
#include <unordered_map>

const int maxn = 1e5 + 3;

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

typedef std::list<std::pair<int, int> > blist_t;
blist_t bl[maxn];

void refineBlist(blist_t &blist) {
	if(blist.size() <= 1)
		return;
	blist_t::iterator i = blist.begin(), j;
	for(; i != blist.end(); i = j) {
		j = i; ++j;
		while(j != blist.end() && j->first == i->first) {
			blist.erase(j);
			j = i; ++j;
		}
	}
}

std::unordered_map<int, int> mp;

void dfs(int u) {
	int prev_mp = -1;
	if(u > 1) {
		blist_t &blist = (bl[u] = bl[pa[u]]);
		for(blist_t::iterator i = blist.begin(); i != blist.end(); ++i)
			i->first &= we[u];
		blist.push_back({we[u], de[u]});
		refineBlist(blist);

		if(mp.find(we[u]) != mp.end())
			prev_mp = mp[we[u]];
		mp[we[u]] = de[u];

		int l, r = de[u] + 1, pos;
		for(blist_t::reverse_iterator i = blist.rbegin(); i != blist.rend(); ++i) {
			l = i->second;
			if(mp.find(i->first) != mp.end()) {
				pos = mp[i->first];
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
		if(prev_mp >= 0)
			mp[we[u]] = prev_mp;
		else
			mp.erase(we[u]);
	}
}

int main() {
	int t;
	scanf("%d", &t);
	while(t--) {
		scanf("%d", &n);
		epl = 0;
		memset(el, 0, sizeof(edge *) * (n + 1));
		for(int i = 1, u, v, w; i < n; ++i) {
			scanf("%d%d%d", &v, &u, &w);
			addEdge(u, v, w);
		}
		ans = 0;
		dfs(1);
		printf("%lld\n", ans);
	}
	return 0;
}
