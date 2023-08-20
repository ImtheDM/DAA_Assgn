#include <time.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
struct Point
{
    float x, y;
};

struct HalfEdge
{
    Point start;
    Point end;
    HalfEdge *twin;
    HalfEdge *next;
    HalfEdge *prev;
    int face;
};

struct Face
{
    HalfEdge *edge;
};

struct DCEL
{
    vector<Point> vertices;
    vector<HalfEdge> edges;
    vector<Face> faces;
};

void decompose(DCEL &d, vector<Point> &vertices)
{
    // Create the convex hull
    d.vertices = vertices;
    int n = vertices.size();
    int currFace = 0;

    // Add all the edges
    for (int i = 0; i < n; i++)
    {
        HalfEdge e;
        e.start = vertices[i];
        e.end = vertices[(i + 1) % n];
        e.face = currFace;
        d.edges.push_back(e);
    }

    // Add all the faces
    for (int i = 0; i < n; i++)
    {
        Face f;
        f.edge = &d.edges[i];
        d.faces.push_back(f);
        currFace++;
    }

    // Link the edges
    for (int i = 0; i < n; i++)
    {
        HalfEdge *e1 = &d.edges[i];
        HalfEdge *e2 = &d.edges[(i + 1) % n];

        e1->next = e2;
        e2->prev = e1;

        e1->twin = e2;
        e2->twin = e1;
    }
}

bool angle(Point p1, Point p2, Point p3)
{
    // vectors from p1 to p2 and p3 to p2
    float v1x =  p1.x-p2.x;
    float v1y =  p1.y-p2.y;
    float v2x =  p3.x-p2.x;
    float v2y =   p3.y-p2.y;
    float cross = v1x * v2y - v1y * v2x;
    if (cross>=0)return true;
    return false;
}

bool notch(const Point &p1, const Point &p2, const Point &p3)
{
    if (angle(p1, p2, p3) ==false)
    {
        return true;
    }
    return false;
}
Point next(const vector<Point> &vertices, Point p)
{
    int k;
    for (int i = 0; i < vertices.size(); i++)
    {
        if (vertices[i].x == p.x && vertices[i].y == p.y)
        {
            k = i;
            break;
        }
    }
    return vertices[(k + 1) % vertices.size()];
}
int side(const Point &p1, const Point &p2, const Point &p3)
{
    float val = (p2.y - p1.y) * (p3.x - p2.x) - (p2.x - p1.x) * (p3.y - p2.y);
    if (val >= 0)
        return 0;
    else
        return -1;
}

vector<float> rectangele(const vector<Point> convex_polygon)
{
    vector<float> ans;
    float min_x = convex_polygon[0].x;
    float min_y = convex_polygon[0].y;
    float max_x = convex_polygon[0].x;
    float max_y = convex_polygon[0].y;
    for (int i = 1; i < convex_polygon.size(); i++)
    {
        if (convex_polygon[i].x < min_x)
            min_x = convex_polygon[i].x;
        if (convex_polygon[i].x > max_x)
            max_x = convex_polygon[i].x;
        if (convex_polygon[i].y < min_y)
            min_y = convex_polygon[i].y;
        if (convex_polygon[i].y > max_y)
            max_y = convex_polygon[i].y;
    }
    ans.push_back(min_x);
    ans.push_back(min_y);
    ans.push_back(max_x);
    ans.push_back(max_y);
    return ans;
}
vector<Point> remove_duplicates(vector<Point> &asd)
{
    for (int i = 0; i < asd.size(); i++)
    {
        for (int j = i + 1; j < asd.size(); j++)
        {
            if (asd[i].x == asd[j].x && asd[i].y == asd[j].y)
            {
                asd.erase(asd.begin() + i);
                i--;
            }
        }
    }
    return asd;
}
vector<vector<Point> > func(vector<Point> &vertices)
{
    vector<vector<Point> > result;
    vector<Point> convex_polygon, notch_list;
    // L0 fv1g; m 1.
    convex_polygon.push_back(vertices[0]);
    int i = 1;
    int vertices_size = vertices.size();
    Point p2,p1;
    int m = 0;
    int flag= 0;
    // clock start time 
    clock_t start = clock();
    while ( vertices.size() > 3)
    {
        
        m++;
        // p1 last of convex_polygon and p2 next in vertices
        i = 1;
        if(flag == 0)
        {
            p1 = convex_polygon[0];
            p2 = next(vertices, p1);
            convex_polygon.push_back(p2);
            convex_polygon = remove_duplicates(convex_polygon);
        }
        Point p3 = next(vertices, vertices[i]); // check 3.2
        while (angle(vertices[(i - 1) % vertices.size()], vertices[i], vertices[(i + 1) % vertices.size()])  && angle(vertices[i], vertices[(i + 1) % vertices.size()], p1)  && angle(vertices[(i + 1) % vertices.size()], p1, p2) )
        {
            convex_polygon.push_back(vertices[(i + 1) % vertices.size()]);
            cout<<"as "<<convex_polygon[convex_polygon.size()-1].x<<" "<<convex_polygon[convex_polygon.size()-1].y<<endl;
            convex_polygon = remove_duplicates(convex_polygon);
            i++;
            p3 = next(vertices, vertices[i]); // check 3.3.1
            // break;
        }

        if (convex_polygon.size() == vertices.size())
        {
            result.push_back(convex_polygon);
            cout<<endl;
            for(int i =0 ;i<convex_polygon.size();i++)
            {
                cout<<"asd "<<convex_polygon[i].x<<" "<<convex_polygon[i].y<<" "<<endl;
            }
            cout<<endl;
            return result;
        }
        if (convex_polygon.size() != vertices.size()) // check 3.4.1
        {
            for (int i = 0; i < vertices.size(); i++)
            {
                if (notch(vertices[i], vertices[(i + 1) % vertices.size()], vertices[(i + 2) % vertices.size()]))
                {
                    // vector<float> temp = rectangele(convex_polygon);
                    // if (vertices[i].x >= temp[0] && vertices[i].x <= temp[2] && vertices[i].y >= temp[1] && vertices[i].y <= temp[3])
                        {
                            notch_list.push_back(vertices[(i + 1) % vertices.size()]);
                        }
                }
            }
            vector<float> temp = rectangele(convex_polygon);
            for (int i = 0; i < notch_list.size(); i++)
            {
                if (notch_list[i].x < temp[0] && notch_list[i].x > temp[2] && notch_list[i].y < temp[1] && notch_list[i].y > temp[3])
                {
                    notch_list.erase(notch_list.begin() + i);
                    i--;
                }
            }
            if (notch_list.size() > 0)
            {
                // convex_polygon notch_list
                Point vert = convex_polygon[1];
                int side_of_convex = side(convex_polygon[0], convex_polygon[convex_polygon.size() - 1], vert);

                for (int i = 0; i < notch_list.size(); i++)
                {
                    if (side_of_convex != side(convex_polygon[0], convex_polygon[convex_polygon.size() - 1], notch_list[i]))
                    {

                        notch_list.erase(notch_list.begin() + i); // check
                        i--;
                    }
                }
                for (int i = 0; i < notch_list.size(); i++)
                {
                    side_of_convex = side(convex_polygon[0], notch_list[i], vert);
                    for (int j = 1; j < convex_polygon.size(); j++)
                    {
                        if (side_of_convex != side(convex_polygon[0], notch_list[i], convex_polygon[j]))
                        {
                            convex_polygon.erase(convex_polygon.begin() + j); // check
                            break;
                        }
                    }
                }
                if(convex_polygon.size()==2)
                {
                    flag++;
                    p1 = p2;
                    p2 = next(vertices, p1);
                    convex_polygon.clear();
                    convex_polygon.push_back(p1);
                    convex_polygon.push_back(p2);
                    convex_polygon = remove_duplicates(convex_polygon);

                    // left shift vertices
                    int num_shift=1;
                    for(int i=0;i<num_shift;i++)
                    {
                        Point temp1= vertices[0];
                        for(int j=0;j<vertices.size()-1;j++)
                        {
                            vertices[j]=vertices[j+1];
                        }
                        vertices[vertices.size()-1]=temp1;
                    }
                    // remove duplicates
                    for(int i=0;i<vertices.size();i++)
                    {
                        for(int j=i+1;j<vertices.size();j++)
                        {
                            if(vertices[i].x==vertices[j].x && vertices[i].y==vertices[j].y)
                            {
                                vertices.erase(vertices.begin()+j);
                                vertices = remove_duplicates(vertices);

                                j--;
                            }
                        }
                    }
                }
            }
        }
        
        if (convex_polygon[convex_polygon.size() - 1].x != p2.x || convex_polygon[convex_polygon.size() - 1].y != p2.y)
        {
            if(convex_polygon.size()>2)
            {
                result.push_back(convex_polygon);
            }
            Point first = convex_polygon[0], last = convex_polygon[convex_polygon.size() - 1];
            // int size_convex_polygon = convex_polygon.size();
            // remove convex polygon from vertcies
            
            for (int i = 0; i < convex_polygon.size(); i++)
            {
                for (int j = 0; j < vertices.size(); j++)
                {
                    if (convex_polygon[i].x == vertices[j].x && convex_polygon[i].y == vertices[j].y)
                    {
                        vertices.erase(vertices.begin() + j);
                        i=0;
                        break;
                    }
                }
            }
            vertices.insert(vertices.begin(), last);
            vertices.insert(vertices.begin(), first);
            vertices = remove_duplicates(vertices);
            cout<<endl;
            for (int i=0;i<vertices.size();i++)
            {
                cout<<i<<" "<<vertices[i].x<<" "<<vertices[i].y<<endl;
            }

            convex_polygon.clear();
            convex_polygon.push_back(first);
            convex_polygon = remove_duplicates(convex_polygon);
            vertices_size = vertices.size();
            notch_list.clear();
        }
        else {
            p1 = p2;
            p2= next(vertices,p1);  
            convex_polygon = remove_duplicates(convex_polygon);
        }
        if((clock()-start )/CLOCKS_PER_SEC >10)
        {
            break;
        }
    }
    if(vertices.size()==3)
        {result.push_back(vertices);}
    
    return result;
}

int main()
{
    vector<Point> vertices;
    int n;
    Point p;
    ifstream inFile("input.txt");
    inFile >> n;
    for (int i = 0; i < n; i++)
    {
        inFile >> p.x >> p.y;
        vertices.push_back(p);
    }
    inFile.close();

    DCEL d;
    decompose(d, vertices);
    // print decomposition
    // cout << "Vertices: ";
    // for (auto v : d.vertices)
    // {
    //     cout << "(" << v.x << ", " << v.y << ") ";
    // }
    // cout<<endl;

    // cout << "Edges: ";
    // for (auto e : d.edges)
    // {
    //     cout << "((" << e.start.x << ", " << e.start.y << "), (" << e.end.x << ", " << e.end.y << ")) ";
    // }
    // cout<<endl;

    // cout << "Faces: ";
    // for (auto f : d.faces)
    // {
    //     cout << "( " << f.edge->start.x << ", " << f.edge->start.y << " ) ";
    // }
    // cout<<endl;

    vector<vector<Point> > result = func(vertices);
    ofstream outFile("output.txt");
    for (int i = 0; i < result.size(); i++)
    {
        for (int j = 0; j < result[i].size(); j++)
        {
            outFile << result[i][j].x << " " << result[i][j].y << endl;
        }
        outFile << endl;
    }
    outFile.close();
    cout<<endl;
    for (int i = 0; i < result.size(); i++)
    {
        for (int j = 0; j < result[i].size(); j++)
        {
            cout << "x " << result[i][j].x << " y " << result[i][j].y << endl;
        }
        cout << endl;
    }

    return 0;
}
