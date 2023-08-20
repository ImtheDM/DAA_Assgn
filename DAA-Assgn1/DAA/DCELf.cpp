#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Vertex;
class HalfEdge;

class Vertex {
	public:
	double x, y;
	HalfEdge *incidentEdge;
	Vertex(double X, double Y) {
		x = X;
		y = Y;
	}
};

class HalfEdge {
	public:
	Vertex *origin;
	HalfEdge *twin;
	HalfEdge *next;
	HalfEdge *prev;

	HalfEdge(Vertex *v) {
		origin = v;
		next = NULL;
		twin = NULL;
	}
	HalfEdge(Vertex *v, HalfEdge *e) {
		origin = v;
		next = NULL;
		twin = e;
		e->twin = this;
	}
};

class Face {
	public:
	HalfEdge *incidentEdge;
};

class DCEL {
	public:
	int n;
	Vertex *startPoint;
	HalfEdge *edge;
	Face f;

	DCEL() {
		ifstream fi;
		fi.open("input.txt", ios::in);

		double x, y;
		fi >> x;
		fi >> y;

		Vertex *v = new Vertex(x, y);
		HalfEdge *edge1 = new HalfEdge(v);
		HalfEdge *edge2 = new HalfEdge(v);	
		edge = edge2;
		v->incidentEdge = edge1;
		startPoint = v;
		n = 1;

		Vertex *vertice = v;
		HalfEdge *prevEdge = edge2;
		while(true) {
			fi >> x;
			fi >> y;
			if (fi.eof())
				break;
			n++;
			Vertex *v = new Vertex(x, y);
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

		vertice->incidentEdge->next = startPoint->incidentEdge;
		startPoint->incidentEdge->prev = vertice->incidentEdge;
		edge->next = prevEdge;
		prevEdge->prev = edge;
		vertice->incidentEdge->twin = edge;
		edge->twin = vertice->incidentEdge;

		edge = startPoint->incidentEdge;
		f.incidentEdge = edge;
		cout << prevEdge->prev->twin->origin->y;
	};

};

int main() {
	DCEL dcel;

	// Do something with the DCEL

	return 0;
}
