#include <iostream>
#include <bits/stdc++.h>
using namespace std;

int maxFlow = 0;
bool pathFlag = false, flag = false;

void augment(int b, vector<int *> &path)
{
	for (auto e : path)
	{
		*e -= b;
	}
}

bool pathTracer(vector<vector<pair<int, int>>> &graph, int s, int t, vector<bool> visited, int delta, int bottleneck, vector<int *> &path)
{
	visited[s] = true;
	if (s == t)
	{
		augment(bottleneck, path);
		maxFlow += bottleneck;
		cout<<"One path complete"<<endl;
		path.clear();
		return true;
	}
	for (int i = 0; i < graph[s].size(); i++)
	{
		int v = graph[s][i].first;
		cout <<s<<" " <<v << endl;
		int capacity = graph[s][i].second;
		cout<<"visited "<<v<<" "<<visited[v]<<endl;
		cout <<"capacity between"<<s<<" & " <<v <<" "<<capacity<< endl;
		if (!visited[v] && capacity >= delta)
		{
		    cout <<s<<" in if " <<v << endl;
			path.push_back(&graph[s][i].second);
			if (pathTracer(graph, v, t, visited, delta, min(bottleneck, capacity), path))
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
	vector<int *>v;
	while (delta >= 1)
	{
		 cout <<"delta "<< delta << endl;
		flag = pathTracer(graph, s, t, visited, delta, INT_MAX, v);
		while (flag)
			flag = pathTracer(graph, s, t, visited, delta, INT_MAX, v);
		delta >>= 1;
	}
	return maxFlow;
}

int main()
{

	ifstream fi;
	fi.open("input.txt", ios::in);
	ofstream fo;
	int n;
	fi >> n;
	vector<vector<pair<int, int>>> polygon(n+1);
	double start, end, flow;
	while (fi >> start >> end >> flow)
	{
		polygon[start].push_back({end, flow});
		//polygon[end].push_back({start, 0});
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
