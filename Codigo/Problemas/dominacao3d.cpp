// DP de Dominacao 3D
//
// Computa para todo ponto i,
// dp[i] = 1 + max_{j dominado por i} dp[j]
// em que ser dominado eh ter as 3 coordenadas menores
// Da pra adaptar facil para outras dps
//
// O(n log^2 n), O(n) de memoria

void lis2d(vector<vector<tuple<int, int, int>>>& v, vector<int>& dp, int l, int r) {
	if (l == r) {
		for (int i = 0; i < v[l].size(); i++) {
			int ii = get<2>(v[l][i]);
			dp[ii] = max(dp[ii], 1);
		}
		return;
	}
	int m = (l+r)/2;
	lis2d(v, dp, l, m);

	vector<tuple<int, int, int>> vv[2];
	vector<int> Z;
	for (int i = l; i <= r; i++) for (auto it : v[i]) {
		vv[i > m].push_back(it);
		Z.push_back(get<1>(it));
	}
	sort(vv[0].begin(), vv[0].end());
	sort(vv[1].begin(), vv[1].end());
	sort(Z.begin(), Z.end());
	auto get_z = [&](int z) { return lower_bound(Z.begin(), Z.end(), z) - Z.begin(); };
	vector<int> seg(2*Z.size());

	int i = 0;
	for (auto it : vv[1]) {
		int y, z, id;
		tie(y, z, id) = it;
		while (i < vv[0].size() and get<0>(vv[0][i]) < y) {
			int y2, z2, id2;
			tie(y2, z2, id2) = vv[0][i++];
			int p = get_z(z2) + Z.size();
			seg[p] = max(seg[p], dp[id2]);
			while (p /= 2) seg[p] = max(seg[2*p], seg[2*p+1]);
		}
		int q = 0, a = 0, b = get_z(z)-1;
		for (a += Z.size(), b += Z.size(); a <= b; ++a/=2, --b/=2) {
			if (a%2 == 1) q = max(q, seg[a]);
			if (b%2 == 0) q = max(q, seg[b]);
		}
		dp[id] = max(dp[id], q + 1);
	}
	lis2d(v, dp, m+1, r);
}

vector<int> solve(vector<tuple<int, int, int>> v) {
	int n = v.size();
	vector<tuple<int, int, int, int>> vv;
	for (int i = 0; i < n; i++) {
		int x, y, z;
		tie(x, y, z) = v[i];
		vv.emplace_back(x, y, z, i);
	}
	sort(vv.begin(), vv.end());

	vector<vector<tuple<int, int, int>>> V;
	for (int i = 0; i < n; i++) {
		int j = i;
		V.emplace_back();
		while (j < n and get<0>(vv[j]) == get<0>(vv[i])) {
			int x, y, z, id;
			tie(x, y, z, id) = vv[j++];
			V.back().emplace_back(y, z, id);
		}
		i = j-1;
	}
	vector<int> dp(n);
	lis2d(V, dp, 0, V.size()-1);
	return dp;
}