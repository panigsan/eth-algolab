// Week 1 - False Coin
#include <iostream>
#include <vector>

using namespace std;

void testcase() {
	int n, k, p, c;
	cin >> n >> k;
	// holds all left and right coins for each weighting
	vector<vector<int> > ls(k, vector<int>()), rs(k, vector<int>());
	// hold outcome of each weighting
	vector<char> outs(k);
	
	for(int i = 0; i < k; ++i) {
		cin >> p;
		for(int j = 0; j < p; ++j) {
			cin >> c;
			ls[i].push_back(c - 1);
		}
		for(int j = 0; j < p; ++j) {
			cin >> c;
			rs[i].push_back(c - 1);
		}
		cin >> outs[i];
	}

	// flag for coin occurrence in the weightings and for suspicioucy of coin
	vector<bool> suspect(n, true);
	vector<bool> occurrs_left(n, 0), occurrs_right(n, 0);
	// Process each weighting
	for(int i = 0; i < k; ++i) {
		if(outs[i] == '=') {
			for(int j = 0; j < ls[i].size(); ++j) {
				suspect[ls[i][j]] = false;
				suspect[rs[i][j]] = false;
			}
		}
		else {
			vector<bool> occurs(n, false);
			for(int j = 0; j < ls[i].size(); ++j) {
				occurs[ls[i][j]] = true;
				occurs[rs[i][j]] = true;
				if(outs[i] == '<') {
					occurrs_left[ls[i][j]] = true;
					occurrs_left[rs[i][j]] = true;
				}
				else {
					occurrs_right[ls[i][j]] = true;
					occurrs_left[rs[i][j]] = true;
				}
			}
			// Rule out all non occuring coins
			for(int j = 0; j < n; ++j)
				if(!occurs[j])
					suspect[j] = false;
		}
	}

	// Rule out coins appearing on both sides
	for(int i = 0; i < n; ++i) {
		if(occurrs_left[i] && occurrs_right[i]) {
			suspect[i] = false;
		}
	}

	// Find possible false coins
	vector<int> false_coins;
	for(int i = 0; i < n; ++i) {
		if(suspect[i]) {
			false_coins.push_back(i);
		}
	}

	// output
	if(false_coins.size() == 1)
		cout << false_coins[0] + 1 << endl;
	else
		cout << "0" << endl;
}

int main() {
	int t;
	cin >> t;
	while(t--)
		testcase();
}