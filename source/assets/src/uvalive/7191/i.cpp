#include <bits/stdc++.h>

typedef long long ll;

const int maxv = 2e4 + 3, maxe = 2e5 + 3;

const ll inf = 0x7f7f7f7f7f7f7f7f;

namespace NetworkFlow {
	struct edge {
		int v;
		ll c;
		edge *n;
		inline edge *rev();
		inline void flow(const ll &f) {
			c -= f;
			rev()->c += f;
		}
	} ep[maxe * 2], *el[maxv];

	inline edge *edge::rev() {
		return &ep[(this - ep) ^ 1];
	}

	int V, E, epl;

	edge *addEdge(int u, int v, ll c) {
		el[u] = &(ep[epl] = {v, c, el[u]});
		el[v] = &(ep[epl + 1] = {u, 0, el[v]});
		epl += 2;
		++E;
		V = std::max(V, std::max(u, v) + 1);
		return el[u];
	}

	void clear() {
		memset(el, 0, sizeof(edge *) * (V + 1));
		V = E = epl = 0;
	}
}

namespace ISAP {
	using namespace NetworkFlow;

	edge *pi[maxv], *cedge[maxv];
	int d[maxv], cntd[maxv], S, T;

	void revBFS() {
		std::queue<int> q;
		memset(cntd, 0, sizeof(int) * (V + 1));

		std::fill(d, d + V, V);
		d[T] = 0;
		cntd[V] = V - 1;
		cntd[0] = 1;

		q.push(T);
		int u;
		while(!q.empty()) {
			u = q.front();
			q.pop();
			for(edge *e = el[u]; e; e = e->n)
			if(e->rev()->c > 0 && d[e->v] >= V) {
				--cntd[d[e->v]];
				++cntd[d[e->v] = d[u] + 1];
				q.push(e->v);
			}
		}
	}

	ll augment() {
		ll f = inf;
		for(edge *e = pi[T]; e; e = pi[e->rev()->v])
		if(f > e->c)
			f = e->c;
		for(edge *e = pi[T]; e; e = pi[e->rev()->v])
			e->flow(f);
		return f;
	}

	int relabel(int u) {
		int &fm = cntd[d[u]], md = V - 1;
		for(edge *e = el[u]; e; e = e->n)
		if(e->c > 0 && md > d[e->v])
			md = d[e->v];
		--fm;
		++cntd[d[u] = md + 1];
		return fm;
	}

	ll maxFlow(int _S, int _T) {
		S = _S; T = _T;
		ll flow = 0;
		revBFS();
		memcpy(cedge, el, sizeof(edge *) * V);
		pi[S] = NULL;
		for(int u = S; d[S] < V;) {
			edge *&e = cedge[u];
			for(; e; e = e->n)
			if(e->c > 0 && d[u] == d[e->v] + 1)
				break;
			if(e) {
				pi[e->v] = e;
				u = e->v;
				if(u == T) {
					flow += augment();
					u = S;
				}
			} else {
				e = el[u];
				if(relabel(u) > 0) {
					if(u != S)
						u = pi[u]->rev()->v;
				} else {
					break;
				}
			}
		}
		return flow;
	}
}

namespace BoundedFlow {
	int S, T;
	ll pf[maxv], tot;
	void clear() {
		memset(pf, 0, sizeof(ll) * (NetworkFlow::V + 2));
		NetworkFlow::clear();
	}

	void addEdge(int u, int v, const ll &up, const ll &low) {
		NetworkFlow::addEdge(u, v, up - low);
		pf[v] += low;
		pf[u] -= low;
	}

	void init() {
		using NetworkFlow::V;
		S = V;
		T = V + 1;
		V += 2;
		tot = 0;
		for(int i = 0, j = V - 2; i < j; ++i) {
			if(pf[i] < 0) {
				NetworkFlow::addEdge(i, T, -pf[i]);
			} else if(pf[i] > 0) {
				NetworkFlow::addEdge(S, i, pf[i]);
				tot += pf[i];
			}
		}
	}

	bool solvable() {
		return ISAP::maxFlow(S, T) == tot;
	}
}

using namespace NetworkFlow;

const int delta[][2] = {
	{1, 1}, {-1, 1}, {1, -1}, {-1, -1}
};
int n, m, a, b;
char s[103][103], t[103][103];

bool isRed(int x, int y) {
	return ~(x / a) & 1;
}

bool vis[maxv], rvis[maxv];

void bfs(int S) {
	std::queue<int> q;
	q.push(S);
	vis[S] = true;
	while(!q.empty()) {
		int u = q.front();
		q.pop();
		for(edge *e = el[u]; e; e = e->n)
		if(e->c && !vis[e->v]) {
			q.push(e->v);
			vis[e->v] = true;
		}
	}
}

void rbfs(int S) {
	std::queue<int> q;
	q.push(S);
	rvis[S] = true;
	while(!q.empty()) {
		int u = q.front();
		q.pop();
		for(edge *e = el[u]; e; e = e->n)
		if(e->rev()->c && !rvis[e->v]) {
			q.push(e->v);
			rvis[e->v] = true;
		}
	}
}

inline bool onlyWhite(int i, int j) {
	if(isRed(i, j))
		return vis[i * m + j];
	else
		return rvis[i * m + j];
}

void solve() {
	clear();

	scanf("%d%d%d%d", &n, &m, &a, &b);
	for(int i = 0; i < n; ++i)
		scanf("%s", s[i]);

	int S = n * m, T = S + 1;

	ll ans = 0;

	for(int i = 0; i < n; ++i) {
		for(int j = 0; j < m; ++j) {
			for(int k = 0; k < 4; ++k) {
				int x = i + a * delta[k][0];
				int y = j + b * delta[k][1];
				if(x < 0 || x >= n || y < 0 || y >= m)
					continue;
				addEdge(i * m + j, x * m + y, 1);
				++ans;
			}
		}
	}

	ans /= 2;

	for(int i = 0; i < n; ++i) {
		for(int j = 0; j < m; ++j) {
			if(s[i][j] == '.')
				continue;
			if((s[i][j] == 'B') ^ isRed(i, j))
				addEdge(S, i * m + j, inf);
			else
				addEdge(i * m + j, T, inf);
		}
	}

	ans -= ISAP::maxFlow(S, T);

	memset(vis, 0, sizeof(vis[0]) * V);
	memset(rvis, 0, sizeof(rvis[0]) * V);

	bfs(S);
	rbfs(T);

	printf("%lld\n", ans);
	for(int i = 0; i < n; ++i) {
		for(int j = 0; j < m; ++j) {
			// choose 'B' greedily
			t[i][j] = onlyWhite(i, j) ? 'W' : 'B';
			if(vis[i * m + j] || rvis[i * m + j])
				continue;

			// if it is not determined to be B/W according to residual graph,
			// force an edge like `addEdge` above.

			// the newly added edge wouldn't update the answer,
			// so just make a BFS for the vertex.
			if((t[i][j] == 'B') ^ isRed(i, j))
				bfs(i * m + j);
			else
				rbfs(i * m + j);
		}
		t[i][m] = 0;
		printf("%s\n", t[i]);
	}
}

int main() {
	int T;
	scanf("%d", &T);
	while(T--)
		solve();
	return 0;
}
