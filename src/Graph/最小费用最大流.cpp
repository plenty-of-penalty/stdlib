// cost_flow
// updated on 2023/11/23
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <queue>
#include <vector>
typedef long long ll;
typedef unsigned un;
typedef pair<ll, ll> pll;
typedef string str;
ll read() {
  ll f = 1, x = 0;
  char c = getchar();
  while (c < '0' || c > '9') {
    if (c == '-') f = -1;
    c = getchar();
  }
  while (c >= '0' && c <= '9') x = x * 10 + c - '0', c = getchar();
  return f * x;
}
ll max(ll a, ll b) { return a > b ? a : b; }
ll min(ll a, ll b) { return a < b ? a : b; }
bool umax(ll &a, ll b) {
  if (b > a) return a = b, 1;
  return 0;
}
bool umin(ll &a, ll b) {
  if (b < a) return a = b, 1;
  return 0;
}
ll abs(ll x) { return x > 0 ? x : -x; }
// poplib start
const int N = 5e3 + 9, M = 2e5 + 9;
const ll INF = 1ll << 58;
struct edge {
  int v;
  ll w, cost, nxt;
} e[M << 1 | 1];
ll cnt = 1, last[N], cur[N];
void adde(int u, int v, ll w, ll c) {
  e[++cnt] = {v, w, c, last[u]}, last[u] = cnt;
  e[++cnt] = {u, 0, -c, last[v]}, last[v] = cnt;
}
ll dis[N];
bool vis[N], inq[N];
bool spfa(ll s, ll t, ll n) {
  for (ll i = 1; i <= n; ++i) vis[i] = 0, dis[i] = INF, cur[i] = last[i];
  queue<ll> q;
  dis[s] = 0;
  q.push(s);
  while (q.size()) {
    ll u = q.front();
    inq[u] = 0;
    q.pop();
    for (ll i = last[u]; i; i = e[i].nxt) {
      ll v = e[i].v;
      if (e[i].w > 0 && umin(dis[v], dis[u] + e[i].cost))
        if (!inq[v]) q.push(v), inq[v] = 1;
    }
  }
  return dis[t] < INF;
}
ll sumc = 0;
ll ex_flow(ll u, ll t, ll flow = INF) {
  if (u == t) return flow;
  vis[u] = 1;
  ll f = 0;
  for (ll &i = cur[u]; i; i = e[i].nxt) {
    ll v = e[i].v;
    if (e[i].w > 0 && !vis[v] && dis[v] == dis[u] + e[i].cost) {
      ll tmp = ex_flow(v, t, min(e[i].w, flow - f));
      e[i].w -= tmp, e[i ^ 1].w += tmp;
      sumc += tmp * e[i].cost;
      f += tmp;
      if (f == flow) return f;
    }
  }
  return f;
}
pll min_cost_zkw(ll s, ll t, ll n) {
  ll res = 0;
  while (spfa(s, t, n)) res += ex_flow(s, t);
  return pll(res, sumc);
}
// poplib end
int main() {
  ll n = read(), m = read();
  for (ll i = 1; i <= m; ++i) {
    ll u = read(), v = read(), w = read(), c = read();
    adde(u, v, w, c);
  }
  pll res = min_cost_zkw(1, n, n);
  printf("%lld %lld", res.first, res.second);
}