#include <iostream>
#include <bits/stdc++.h>
using namespace std;

bool pathFlag = false, flag = false;
int n;
vector<pair<int, int>> edges;
void augment(int b, vector<int *> &path)
{
	for (auto e : path)
	{
		*e -= b;
	}
}

void finalEdges(vector<bool> visited){
    vector<int> temp;
    for(int i=0;i<n;i++){
        if(visited[i]){
            temp.push_back(i);
        }
    }
    edges.push_back({temp[0],temp[1]});
}

bool pathTracer(vector<vector<pair<int, int>>> &graph, int s, int t, vector<bool> visited, int delta, int bottleneck, vector<int *> &path)
{
	visited[s] = true;
	if (s == t)
	{
		augment(bottleneck, path);
        finalEdges(visited);
		path.clear();
		return true;
	}
	for (int i = 0; i < graph[s].size(); i++)
	{
		int v = graph[s][i].first;
		int capacity = graph[s][i].second;
		if (!visited[v] && capacity >= delta)
		{
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

void fordFulkerson(vector<vector<pair<int, int>>> &graph, int s, int t)
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
		flag = pathTracer(graph, s, t, visited, delta, INT_MAX, v);
		while (flag)
			flag = pathTracer(graph, s, t, visited, delta, INT_MAX, v);
		delta >>= 1;
	}
}

void printEdges(vector<pair<int, int>> edges){
    ofstream outputFile("output3.txt");
    for(auto i:edges){
        outputFile << i.first << " " << i.second << endl;
    }
    outputFile.close();
}

void transform(vector<vector<pair<int, int>>> &adjList) {
    int V = adjList.size();
    adjList.resize(V+2);
    for(int i = 0; i < V; i++) {
        if(adjList[i].size() == 0){
            adjList[i].push_back({V+1, 1});  //V+1 is the sink
        }
        else{
            adjList[V].push_back({i, 1});   //V is the source
        }
    }
}

int main()
{

	ifstream fi;
	fi.open("input3.txt", ios::in);
	ofstream fo;
	fi >> n;
	vector<vector<pair<int, int>>> polygon(n);
	double start, end, flow;
	while (fi >> start >> end >> flow)
	{
		polygon[start].push_back({end, flow});
        cout<<start<<" "<<end<<" "<<flow<<endl;
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
    transform(polygon);
	fordFulkerson(polygon, 9, 10);
	printEdges(edges);
	return 0;
}
