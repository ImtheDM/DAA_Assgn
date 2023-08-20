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
		// cout << delta << endl;
		flag = pathTracer(graph, s, t, visited, delta, INT_MAX, v);
		while (flag)
			flag = pathTracer(graph, s, t, visited, delta, INT_MAX, v);
		delta >>= 1;
	}
	return maxFlow;
}

vector<int> min_st(vector<vector<pair<int, int>>> polygon) {
    queue<int> q;
    vector<int> v;
    q.push(0);
    vector<bool> visited(polygon.size(), false);
    visited[0] = true;
    while(!q.empty()) {
        int u = q.front();
        q.pop();
        for(int i = 0; i < polygon[u].size(); i++) {
            int v = polygon[u][i].first;
            if(!visited[v] && polygon[u][i].second > 0) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
    for(int i = 0; i < visited.size(); i++) {
        if(visited[i]) v.push_back(i);
    }
    return v;
}

void printAdjList(vector<vector<pair<int, int>>> adjList, int V) {
    ofstream outputFile("output.txt");
    for (int i = 0; i < V; i++) {
        for (auto j : adjList[i]) {
            if(j.second != 0) outputFile << i << " " << j.first << " " << j.second << endl;
        }
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
	fi.open("input.txt", ios::in);
	ofstream fo;
	int n;
	fi >> n;
	vector<vector<pair<int, int>>> polygon(n),fixedPolygon(n),ans(n);
	double start, end, flow;
	while (fi >> start >> end >> flow)
	{
		polygon[start].push_back({end, flow});
        fixedPolygon[start].push_back({end, flow});
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

    // transform(polygon);
	cout << fordFulkerson(polygon, 0, n-1);
	cout << endl;
    // Copy value of polygon in ans
    for(int i = 0; i < polygon.size(); i++) {
        for(int j = 0; j < polygon[i].size(); j++) {
            ans[i].push_back({polygon[i][j].first, polygon[i][j].second});
        }
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
    for(int i = 0; i < polygon.size(); i++) {
        for(int j = 0; j < polygon[i].size(); j++) {
            // cout<<"here"<<endl;
            if(polygon[i][j].second != fixedPolygon[i][j].second) {
                // cout<<"yo"<<endl;
                ans[polygon[i][j].first].push_back({i, abs(polygon[i][j].second - fixedPolygon[i][j].second)});
            }
        }
    }
    printAdjList(ans, n);
    vector<int> st = min_st(polygon);
    //Write the value of min_st in min_st.txt
    ofstream min_st_file("min_st.txt");
    for(int i = 0; i < st.size(); i++) {
        min_st_file << st[i] << endl;
    }
	return 0;
}
