#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <math.h>
#include <algorithm>
#include "DCEL.h"

using namespace std;

bool angle(Vertex *v1, Vertex *v2, Vertex *v3)
{
	double x1 = v1->x - v2->x;
	double y1 = v1->y - v2->y;
	double x2 = v3->x - v2->x;
	double y2 = v3->y - v2->y;

	double val = (x1 * y2 - y1 * x2);

	if (val >= 0)
		return true;
	else
		return false;
}

bool isNotch(Vertex *v1, Vertex *v2, Vertex *v3)
{
	if (!angle(v1, v2, v3))
	{
		return true;
	}
	return false;
}

void findRectangle(double &minX, double &minY, double &maxX, double &maxY, vector<Vertex *> vec)
{
	minX = vec[0]->x;
	minY = vec[0]->y;
	maxX = vec[0]->x;
	maxY = vec[0]->y;
	for (int i = 1; i < vec.size(); i++)
	{
		if (vec[i]->x < minX)
			minX = vec[i]->x;
		if (vec[i]->x > maxX)
			maxX = vec[i]->x;
		if (vec[i]->y < minY)
			minY = vec[i]->y;
		if (vec[i]->y > maxY)
			maxY = vec[i]->y;
	}
}

void remove(vector<Vertex *> &vec, Vertex *v, DCEL d)
{
	while (v != vec[0])
	{
		double minX, minY, maxX, maxY;
		findRectangle(minX, minY, maxX, maxY, vec);
		if (v->x < minX || v->x > maxX || v->y < minY || v->y > maxY)
		{
			//executes if outside rectangle
			v = v->incidentEdge->next->origin;
			continue;
		}
		if (angle(v->incidentEdge->twin->next->twin->origin, v, v->incidentEdge->twin->origin))
		{
			v = v->incidentEdge->next->origin;
			continue;
		}

		double m = (vec[vec.size() - 1]->y - vec[0]->y) / (vec[vec.size() - 1]->x - vec[0]->x);
		double c = (m * vec[0]->x) - vec[0]->y;
		double sign = vec[1]->y - (m * vec[1]->x) + c;
		int pointSign = v->y - (v->x * m) + c;

		if (sign * pointSign > 0)
		{
			// same side
			vec.pop_back();
		}
		else
		{
			v = v->incidentEdge->next->origin;
		}
	}
}

int main()
{
	vector<vector<double>> polygon;
	ifstream fi;
	fi.open("input.txt", ios::in);
	ofstream fo;

	double x, y;
	while (true)
	{
		fi >> x;
		fi >> y;
		if (fi.eof())
			break;
		polygon.push_back({x, y});
	}
	DCEL d;
	d = d.createDCEL(polygon);
	vector<Vertex *> points;
	vector<vector<Vertex *>> L;
	Vertex *p1 = d.startPoint->incidentEdge->twin->origin;
	while (!angle(d.startPoint, p1, p1->incidentEdge->twin->origin))
	{
		d.startPoint = p1;
		p1 = p1->incidentEdge->twin->origin;
	}
	L.push_back({d.startPoint});
	int m = 0;
	while (n >= 3)
	{
		if (L[m][L[m].size()-1]->incidentEdge->origin == L[m][L[m].size()-1]->incidentEdge->next->next->next->origin)
		{
			fo.open("output.txt", ios::out | ios::app);
			Vertex *v = d.startPoint;
			string s = to_string((double)v->x) + " " + to_string((double)v->y) + "\n";
			fo << s;
			v = v->incidentEdge->twin->origin;
			s = to_string((double)v->x) + " " + to_string((double)v->y) + "\n";
			fo << s;
			v = v->incidentEdge->twin->origin;
			s = to_string((double)v->x) + " " + to_string((double)v->y) + "\n";
			fo << s;
			
			fo << endl;
			fo.close();

			break;
		}
		Vertex *p1 = L[m][L[m].size() - 1]->incidentEdge->twin->origin;
		while (!angle(L[m][L[m].size() - 1], p1, p1->incidentEdge->twin->origin))
		{
			L[m][L[m].size() - 1] = p1;
			p1 = p1->incidentEdge->twin->origin;
		}

		points.push_back(L[m][L[m].size() - 1]);
		d.startPoint = points[0];
		points.push_back(points[0]->incidentEdge->next->origin);

		vector<Vertex *> vec;
		vec.push_back(points[0]);
		vec.push_back(points[1]);
		int i = 1;
		points.push_back(points[i]->incidentEdge->next->origin);
		while (angle(points[i - 1], points[i], points[i + 1]) && angle(points[i], points[i + 1], points[0]) && angle(points[i + 1], points[0], points[1]) && vec.size() < n)
		{
			if (find(vec.begin(), vec.end(), points[i + 1]) == vec.end())
			{
				vec.push_back(points[i + 1]);
				points.push_back(points[++i]->incidentEdge->next->origin);
			}
			else
			{
				i++;
			}
		}
		if (vec.size() != n)
		{
			remove(vec, points[points.size() - 1], d);
		}
		if (vec[vec.size() - 1] != points[1])
		{
			for (int k = 0; k < vec.size(); k++)
			{
				fo.open("output.txt", ios::out | ios::app);
				string s = to_string((double)vec[k]->x) + " " + to_string((double)vec[k]->y) + "\n";
				fo << s;
				fo.close();
			}
			fo.open("output.txt", ios::out | ios::app);
			fo << endl;
			fo.close();
		}
	
		L.push_back(vec);
		
		if (vec.size() == n)
			break;
		d.deleteVertices(vec[0], vec[vec.size() - 1]);
		++m;
		vec.clear();
		points.clear();
	}

	fi.close();
	fo.close();
	return 0;
}