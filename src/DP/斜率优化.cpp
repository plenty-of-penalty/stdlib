int sz;
struct line {
  int k, b;
  bool F(int x) { return x * k + b; }
} L[N];
bool cov(line l1, line l2, line l3) { // insert l1, l1 & l2 cover l3
  return 1ll * (l2.b - l1.b) * (l1.k - l3.k) >= 1ll * (l3.b - l1.b) * (l1.k - l2.k);
}
void insert(line l) {
  if (sz && L[sz].k == l.k) {
    // L[sz].b = max(L[sz].b, l.b);
    return;
  }
  while (sz > 1 && cov(l, L[sz], L[sz - 1])) --sz;
  L[++sz] = l;
}