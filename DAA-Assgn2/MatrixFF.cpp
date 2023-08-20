#include <iostream>
#include <bits/stdc++.h>
using namespace std;

int maxFlow = 0;
int n;
bool pathFlag = false, flag = false;

void augment(vector<vector<pair<int, int>>> &polygon, int b, vector<pair<int, int>> path)
{
	for (auto e : path)
	{
		// polygon[e.first][e.second].first += b;
        // polygon[e.second][e.first].first = polygon[e.first][e.second].first;
        // // polygon[e.first][e.second]
        // polygon[e.first][e.second].first = polygon[e.first][e.second].second - polygon[e.first][e.second].first;
        polygon[e.second][e.first].first += b;
        polygon[e.first][e.second].first = polygon[e.first][e.second].second - polygon[e.second][e.first].first;
	}
}
void display(vector<vector<pair<int, int>>> polygon)
{
    for (int i = 0; i < polygon.size(); i++)
	{
		cout << i << ": ";
		for (int j = 0; j < polygon[i].size(); j++)
		{
			cout << polygon[i][j].first << " " << polygon[i][j].second << " ";
		}
		cout << endl;
	}
}
bool pathTracer(vector<vector<pair<int, int>>> &graph, int s, int t, vector<bool> visited, int delta, int bottleneck, vector<pair<int, int>> path)
{
	visited[s] = true;
	if (s == t)
	{
		augment(graph,bottleneck, path);
		maxFlow += bottleneck;
        cout<<"bottleneck: "<<bottleneck<<endl;
        // display(graph);
		cout<<"One path complete"<<endl;
        cout<<"PATH: ";
        for(auto e: path)
        {
            cout<<"["<<e.first<<","<<e.second<<"] ";
        }
		path.clear();
		return true;
	}
	for (int i = 0; i < n; i++)
	{
		if (i == s)
			continue;

		// cout << s << " " << i << endl;
		int rcapacity = graph[s][i].second - graph[i][s].first;
		// cout<<"visited "<< i << " " << visited[i] << endl;
		// cout <<"capacity between"<<s<<" & " << i <<" "<< rcapacity<< endl;
		if (!visited[i] && rcapacity >= delta)
		{
			cout <<s<<" in if " <<i << endl;
			path.push_back({s, i});
			if (pathTracer(graph, i, t, visited, delta, min(bottleneck, rcapacity), path))
			{
				return true;
			}
			path.pop_back();
		}
	}
	return false;
}

int fordFulkerson(vector<vector<pair<int, int>>> &graph, int s, int t)
{
	int n = graph.size();
	int temp = 0;

	for (auto it : graph[s])
	{
		temp = max(temp, it.second);
	}

	int delta = 1;

	while (delta <= temp)
	{
		delta = delta * 2;
	}

	delta = delta / 2;
	vector<bool> visited(n, false);
	vector<pair<int , int >> path;

	while (delta >= 1)
	{
		bool flag;
		do
	       	{
                cout<<"delta: "<<delta;
			flag = pathTracer(graph, s, t, visited, delta, INT_MAX, path);
		} while (flag);
		
		delta >>= 1;
	}
	return maxFlow;
}



int main()
{

	ifstream fi;
	fi.open("input.txt", ios::in);
	ofstream fo;
	fi >> n;
	vector<vector<pair<int, int>>> polygon(n, vector<pair<int, int>>(n, {0, 0}));
	int start, end, capacity;
	while (fi >> start >> end >> capacity)
	{
		polygon[start][end].second = capacity;
        polygon[end][start].second = capacity;
		if (fi.eof())
			break;
	}
	for (int i = 0; i < polygon.size(); i++)
	{
		cout << i << ": ";
		for (int j = 0; j < polygon[i].size(); j++)
		{
			cout << polygon[i][j].first << " " << polygon[i][j].second << " ";
		}
		cout << endl;
	}

	cout << fordFulkerson(polygon, 0, 7);
	cout << endl;

	for (int i = 0; i < polygon.size(); i++)
	{
		cout << i << ": ";
		for (int j = 0; j < polygon[i].size(); j++)
		{
			cout << polygon[i][j].first << " " << polygon[i][j].second << " ";
		}
		cout << endl;
	}

	return 0;
}
