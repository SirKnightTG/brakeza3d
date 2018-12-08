
#ifndef BRAKEDA3D_MATHS_H
#define BRAKEDA3D_MATHS_H


#include "Plane.h"
#include "../Objects/Vector3D.h"
#include "../Objects/Object3D.h"
#include "../Objects/Point2D.h"
#include "../Objects/Camera3D.h"
#include "Texture.h"
#include "../Objects/LightPoint3D.h"
#include "../Objects/Triangle3D.h"

class Maths
{
public:
    static float degreesToRadians(float);
    static float radiansToDegrees(float);

    static int isVector3DClippingPlane(Plane P, Vector3D V);

    static float barycentricSide(int x, int y, Point2D pa, Point2D pb);
    static void getBarycentricCoordinates(float &, float &, float &, int, int, Point2D, Point2D, Point2D);

    static Vertex3D rotateVertex(Vertex3D V, M3 rotation);

    static Vertex3D getCenterVertices(Vertex3D vertices[], int num_vertices);

    static void sortVerticesByX(Vertex3D *vertexes, int N);
    static void sortVerticesByY(Vertex3D *vertexes, int N);

    static void sortVerticesByY(Vertex3D &A, Vertex3D &B, Vertex3D &C);
    static void sortVerticesByX(Vertex3D &A, Vertex3D &B, Vertex3D &C);

    static void sortVertexesByAngles(Vertex3D &A, Vertex3D &B, Vertex3D &C);
    static void sortVertexesByAngles(Vertex3D *vertexes, int N);

    static void sortPointsByY(Point2D &A, Point2D &B, Point2D &C);
    static void sortPointsByX(Point2D &A, Point2D &B, Point2D &C);

    static void VertexSwap(Vertex3D *vertexes, int i, int j);

    static float distanteBetweenpoints(Vertex3D v1, Vertex3D v2);

    static Uint32 mixColor(Uint32 color, float distance, LightPoint3D *lp, Vertex3D Q);

    static Vertex3D crossProduct(Vertex3D, Vertex3D);

    static float getHorizontalAngleBetweenObject3DAndCamera(Object3D *o1, Camera3D *cam);
    static void sortVerticesClockWise(Vertex3D *vertices, int num_vertex);

    static long GetNextActive(long x, long vertexCount, const bool *active);
    static long GetPrevActive(long x, long vertexCount, const bool *active);
    static int TriangulatePolygon(long vertexCount, Vertex3D *vertices, Vertex3D normal, Triangle *triangle, int &ntriangles, Object3D *parent, Texture *texture, bool clipped);
    static bool ClippingPolygon(Vertex3D *input, int ninput, Vertex3D *output, int &noutput, int plane_id , Camera3D *cam);

};


#endif //BRAKEDA3D_MATHS_H
