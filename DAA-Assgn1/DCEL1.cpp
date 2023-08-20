#include <iostream>
#include <vector>

using namespace std;

class Vertex;
class HalfEdge;

class Vertex {
public:
    double x, y;
    HalfEdge* incidentEdge;
};

class HalfEdge {
public:
    Vertex* origin;
    HalfEdge* twin;
    HalfEdge* next;
};

class DCEL {
public:
    vector<Vertex> vertices;
    vector<HalfEdge> halfEdges;
};

DCEL createDCEL(vector<vector<double>>& polygon) {
    DCEL dcel;
    int n = polygon.size();
    dcel.vertices.resize(n);
    dcel.halfEdges.resize(n * 2);

    for (int i = 0; i < n; i++) {
        dcel.vertices[i].x = polygon[i][0];
        dcel.vertices[i].y = polygon[i][1];
    }

    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;

        HalfEdge* edge1 = &dcel.halfEdges[2 * i];
        HalfEdge* edge2 = &dcel.halfEdges[2 * i + 1];

        edge1->origin = &dcel.vertices[i];
        edge1->twin = edge2;
        edge1->next = &dcel.halfEdges[2 * j];

        edge2->origin = &dcel.vertices[j];
        edge2->twin = edge1;
        edge2->next = &dcel.halfEdges[(2 * i) + 1];

        dcel.vertices[i].incidentEdge = edge1;
    }

    return dcel;
}

int main() {
    vector<vector<double>> polygon = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    DCEL dcel = createDCEL(polygon);
	printf("DCEL created");
	//Access vertices through dcel.vertices

    // Do something with the DCEL

    return 0;
}
