#include <bits/stdc++.h>

const int maxv = 1e5, maxe = 2e5;

struct edge {
	int v;
	edge *n;
} ep[maxe], *el[maxv];

bool eban[maxv][4];

int epl;
inline void addEdge(int u, int v) {
	el[u] = &(ep[epl++] = {v, el[u]});
}

const int dxy[4][2] = {
	{-1, 0}, {1, 0}, {0, -1}, {0, 1}
};

int a, b;

inline int label(int x, int y) {
	return (x + a) * (2 * b + 1) + y + b;
}

inline bool inOval(int x, int y) {
	long long xb = x * b, ya = y * a, ab = a * b;
	return xb * xb + ya * ya <= ab * ab;
}

int V, color[maxv];

void buildGraph() {
	V = 0;
	epl = 0;
	memset(el, 0, sizeof(el));
	memset(color, 0, sizeof(color));

	std::queue<std::pair<int, int> > q;
	q.emplace(std::make_pair(0, 0));
	color[label(0, 0)] = 1;
	while(!q.empty()) {
		int x = q.front().first, y = q.front().second;
		int u = label(x, y), v;
		V = std::max(V, u);
		q.pop();
		for(int i = 0; i < 4; ++i)
		if(inOval(x + dxy[i][0], y + dxy[i][1])) {
			v = label(x + dxy[i][0], y + dxy[i][1]);
			if(!eban[u][i])
				addEdge(u, v);
			if(!color[v]) {
				q.emplace(std::make_pair(x + dxy[i][0], y + dxy[i][1]));
				color[v] = color[u] == 1 ? 2 : 1;
			}
		}
	}
}

int timestamp, match[maxv], visit[maxv];

bool hungarian(int u) {
	if(visit[u] == timestamp)
		return false;
	visit[u] = timestamp;
	for(edge *e = el[u]; e; e = e->n) {
		if(visit[e->v] == timestamp)
			continue;
		visit[e->v] = timestamp;
		if(match[e->v] < 0 || hungarian(match[e->v])) {
			match[e->v] = u;
			match[u] = e->v;
			return true;
		}
	}
	return false;
}

std::vector<int> cover;

int minCover() {
	timestamp = 0;
	memset(match, 0xff, sizeof(match));
	memset(visit, 0, sizeof(visit));
	cover.clear();
	int m = 0;
	for(int u = 0; u <= V; ++u)
	if(color[u] == 1) {
		++timestamp;
		if(hungarian(u))
			++m;
	}
	++timestamp;
	for(int u = 0; u <= V; ++u)
	if(color[u] == 1 && match[u] < 0)
		hungarian(u);
	for(int u = 0; u <= V; ++u)
	if(color[u] == 1 && visit[u] != timestamp
			|| color[u] == 2 && visit[u] == timestamp)
		cover.push_back(u);
	return m;
}

void readBanlist() {
	memset(eban, 0, sizeof(eban));
	int m;
	scanf("%d", &m);
	for(int i = 0, j, u, v, x, y; i < m; ++i) {
		scanf("%d%d%d%d", &u, &v, &x, &y);
		for(j = 0; j < 4; ++j)
		if(u + dxy[j][0] == x && v + dxy[j][1] == y) {
			eban[label(u, v)][j] = true;
			break;
		}
		for(j = 0; j < 4; ++j)
		if(x + dxy[j][0] == u &&y + dxy[j][1] == v) {
			eban[label(x, y)][j] = true;
			break;
		}
	}
}

int main() {
	int t;
	scanf("%d", &t);
	while(t--) {
		scanf("%d%d", &a, &b);
		readBanlist();
		buildGraph();
		int ans = minCover();
		printf("%d\n", ans);
		for(int i = 0; i < cover.size(); ++i) {
			printf("%d %d\n", cover[i] / (2 * b + 1) - a, cover[i] % (2 * b + 1) - b);
		}
	}
	return 0;
}
