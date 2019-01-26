// Week 10 - Odd Route
#include <iostream>
#include <vector>
#include <stack>

using namespace std;

typedef pair<int,pair<bool,int> > pipbi;

pipbi make_pipbi(int vertex, bool dist, int weight) {
	return make_pair(vertex, make_pair(dist, weight));
}

void testcase() {
	int n, m, s, t, x, y, w;
	cin >> n >> m >> s >> t;
	// next/weight
	vector<vector<pair<int,int> > > graph(n, vector<pair<int,int> >());
	for(int i = 0; i < m; ++i) {
		cin >> x >> y >> w;
		graph[x].push_back(make_pair(y, w));
	}

	// Modified DFS
	// Visited for distance/weight combinations
	// odd -> true, even -> false
	// 3 : odd/odd, 1 : odd/even, 2 : even/odd, 0 : even/even -> first + 2 * second
	vector<vector<bool> > visited(n, vector<bool>(4, false));
	vector<vector<int> > min_dist(n, vector<int>(4));
	stack<pipbi> waiting;
	waiting.push(make_pipbi(s, false, 0));
	visited[s][0] = true;
	while(!waiting.empty()) {
		pipbi curr = waiting.top();
		waiting.pop();
		bool odd_dist = !curr.second.first;
		for(int i = 0; i < graph[curr.first].size(); ++i) {
			pair<int,int> edge = graph[curr.first][i];
			int next = edge.first, weight = edge.second;
			bool odd_weight = (curr.second.second + weight) % 2;
			if(!visited[next][odd_dist + 2 * odd_weight]) {
				min_dist[next][odd_dist + 2 * odd_weight] = curr.second.second + edge.second;
				visited[next][odd_dist + 2 * odd_weight] = true;
				waiting.push(make_pipbi(next, odd_dist, min_dist[next][odd_dist + 2 * odd_weight]));

			}
			else if(curr.second.second + weight < min_dist[next][odd_dist + 2 * odd_weight]) {
				min_dist[next][odd_dist + 2 * odd_weight] = curr.second.second + weight;
				waiting.push(make_pipbi(next, odd_dist, min_dist[next][odd_dist + 2 * odd_weight]));
			}
		}
	}

	// Output
	if(visited[t][3])
		cout << min_dist[t][3] << endl;
	else
		cout << "no" << endl;

}

int main() {
	int t;
	cin >> t;
	while(t--)
		testcase();
}