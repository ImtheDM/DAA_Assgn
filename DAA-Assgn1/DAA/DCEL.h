#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;

class Vertex;
class HalfEdge;
class DCEL;
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
		vector<pair<HalfEdge *, HalfEdge*>> diagonals;

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