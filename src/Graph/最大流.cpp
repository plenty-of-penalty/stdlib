#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <queue>
#include <vector>
typedef long long ll;
typedef pair<int, int> pii;
typedef string str;
using namespace std;
const ll LINF = 1e18;
// poplib start
const int N = 2e5 + 9;
struct edge {
  int v, nxt;
  ll w;
} e[N << 1 | 1];
int cnt = 1, last[N];
void adde(int u, int v, ll w) {
  e[++cnt].v = v, e[cnt].w = w, e[cnt].nxt = last[u], last[u] = cnt;
  e[++cnt].v = u, e[cnt].w = 0, e[cnt].nxt = last[v], last[v] = cnt;
}
int dep[N], cur[N], Q[N];
bool bfs(int s, int t, int n) {
  // printf("BFS %d\n",s);
  for (int i = 1; i <= n; ++i) dep[i] = 0, cur[i] = last[i];
  int qh = 0, qt = 0;
  Q[qt++] = s, dep[s] = 1;
  while (qh < qt) {
    int u = Q[qh++];
    for (int i = last[u]; i; i = e[i].nxt) {
      int v = e[i].v;
      if (e[i].w && !dep[v]) {
        dep[v] = dep[u] + 1, Q[qt++] = v;
        if (v == t) return 1;
      }
    }
  }
  return 0;
}
ll ex_flow(int u, int t, ll flow = LINF) {
  if (u == t) return flow;
  ll f = 0;
  for (int &i = cur[u]; i; i = e[i].nxt) {
    int v = e[i].v;
    if (e[i].w && dep[v] == dep[u] + 1) {
      ll tmp = ex_flow(v, t, min(e[i].w, flow - f));
      e[i].w -= tmp, e[i ^ 1].w += tmp;
      f += tmp;
      if (f == flow) return f;
    }
  }
  return f;
}
ll Dinic(int s, int t, int n) {
  ll f = 0;
  while (bfs(s, t, n)) f += ex_flow(s, t);
  return f;
}
// poplib end
int main() {
  cin.tie(0)->sync_with_stdio(0);
  int n, m, s, t;
  cin >> n >> m >> s >> t;
  while (m--) {
    int u, v;
    ll w;
    cin >> u >> v >> w;
    adde(u, v, w);
  }
  cout << Dinic(s, t, n) << '\n';
}