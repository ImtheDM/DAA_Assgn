#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <math.h>
#include <algorithm>

using namespace std;

class Vertex;
class HalfEdge;
int n;
class Vertex
{
	public:
		double x, y;
		HalfEdge *incidentEdge;
		Vertex(double X, double Y)
		{
			x = X;
			y = Y;
		}
};

class HalfEdge
{
	public:
		Vertex *origin;
		HalfEdge *twin;
		HalfEdge *next;
		HalfEdge *prev;

		HalfEdge(Vertex *v)
		{
			origin = v;
			next = NULL;
			twin = NULL;
		}
		HalfEdge(Vertex *v, HalfEdge *e)
		{
			origin = v;
			next = NULL;
			twin = e;
			e->twin = this;
		}
};

class Face
{
	public:
		HalfEdge *incidentEdge;
};

class DCEL
{
	public:
		Vertex *startPoint;
		HalfEdge *edge;
		Face f;

		DCEL(){

		};

		DCEL createDCEL(vector<vector<double>> &polygon)
		{
			DCEL dcel;

			Vertex *v = new Vertex(polygon[0][0], polygon[0][1]);
			HalfEdge *edge1 = new HalfEdge(v);
			HalfEdge *edge2 = new HalfEdge(v);
			dcel.edge = edge2;
			v->incidentEdge = edge1;
			dcel.startPoint = v;
			n = polygon.size();

			Vertex *vertice = v;
			HalfEdge *prevEdge = edge2;
			for (int i = 1; i < n; i++)
			{
				Vertex *v = new Vertex(polygon[i][0], polygon[i][1]);
				HalfEdge *edge1 = new HalfEdge(v);
				HalfEdge *edge2 = new HalfEdge(v, vertice->incidentEdge);
				v->incidentEdge = edge1;

				edge2->next = prevEdge;
				prevEdge->prev = edge2;
				vertice->incidentEdge->next = edge1;
				edge1->prev = vertice->incidentEdge;
				vertice = v;
				prevEdge = edge2;
			}

			vertice->incidentEdge->next = dcel.startPoint->incidentEdge;
			dcel.startPoint->incidentEdge->prev = vertice->incidentEdge;
			dcel.edge->next = prevEdge;
			prevEdge->prev = dcel.edge;
			vertice->incidentEdge->twin = dcel.edge;
			dcel.edge->twin = vertice->incidentEdge;

			dcel.edge = dcel.startPoint->incidentEdge;
			dcel.f.incidentEdge = dcel.edge;
			return dcel;
		}

		void display()
		{
			Vertex *v = startPoint;

			do
			{
				cout << v->x << " " << v->y << endl;
				v = v->incidentEdge->twin->origin;
			}while(v != startPoint);

		}

		void deleteVertices(Vertex *v1, Vertex *v2)
		{
			HalfEdge *v1PrevEdge = v1->incidentEdge->prev;
			Vertex *v = v1->incidentEdge->twin->origin;
			while (true)
			{
				Vertex *temp = v->incidentEdge->twin->origin;
				delete v->incidentEdge->prev->twin;
				delete v->incidentEdge->prev;
				if (v != v2)
					delete v;
				else
					break;
				n--;
				v = temp;
			}

			HalfEdge *edge1 = new HalfEdge(v1);
			HalfEdge *edge2 = new HalfEdge(v2, edge1);
			v1->incidentEdge = edge1;
			v1PrevEdge->next = v1->incidentEdge;
			v1->incidentEdge->prev = v1PrevEdge;
			v1->incidentEdge->next = v2->incidentEdge;
			v2->incidentEdge->prev = v1->incidentEdge;
			edge2->next = v1PrevEdge->twin;
			v1PrevEdge->twin->prev = edge2;
			edge2->prev = v2->incidentEdge->twin;
			v2->incidentEdge->twin->next = edge2;

		}
};

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
		if (angle(v->incidentEdge->prev->origin, v, v->incidentEdge->twin->origin))
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
		if (n == 3)
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
