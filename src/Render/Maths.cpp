
#include "../../headers/Render/Maths.h"
#include "../../headers/Render/Drawable.h"
#include "../../headers/Render/M3.h"
#include "../../headers/Render/Transforms.h"
#include "../../headers/Render/EngineBuffers.h"
#include "../../headers/Render/Tools.h"
#include "../../headers/Render/Engine.h"
#include "../../headers/Render/Logging.h"


float Maths::degreesToRadians(float angleDegrees)
{
    return angleDegrees * (float) M_PI / (float) 180.0;;
}

float Maths::radiansToDegrees(float angleRadians)
{
    return angleRadians * (float) 180.0 / (float) M_PI;;
}

Vertex3D Maths::rotateVertex(Vertex3D V, M3 rotation)
{
    Vertex3D B = rotation * V;
    B.u = V.u; B.v = V.v;

    return B;
}

// https://elcodigografico.wordpress.com/2014/03/29/coordenadas-baricentricas-en-triangulos/
float Maths::barycentricSide(int x, int y, Point2D pa, Point2D pb)
{
    return (pa.y - pb.y) * x + (pb.x - pa.x) * y + pa.x * pb.y - pb.x * pa.y;
}

void Maths::getBarycentricCoordinates(float &alpha, float &theta, float &gamma, int x, int y, Point2D v1, Point2D v2, Point2D v3)
{
    alpha = Maths::barycentricSide( x, y, v2, v3 ) / Maths::barycentricSide( v1.x, v1.y, v2, v3 );
    theta = Maths::barycentricSide( x, y, v3, v1 ) / Maths::barycentricSide( v2.x, v2.y, v3, v1 );
    gamma = Maths::barycentricSide( x, y, v1, v2 ) / Maths::barycentricSide( v3.x, v3.y, v1, v2 );
}

/**
 * 0 = dos vértices dentro
 * 1 = ningún vértice dentro
 * 2 = vértice A dentro
 * 3 = vértice B dentro
 */
int Maths::isVector3DClippingPlane(Plane P, Vector3D V)
{
    float min_distance_to_clipping = EngineSetup::getInstance()->FRUSTUM_CLIPPING_DISTANCE;

    if (P.distance(V.vertex1) > min_distance_to_clipping && P.distance(V.vertex2) > min_distance_to_clipping) {
        return 1;
    } else {
        if (P.distance(V.vertex1) > min_distance_to_clipping && P.distance(V.vertex2) < min_distance_to_clipping) {
            return 3;
        }

        if (P.distance(V.vertex2) > min_distance_to_clipping && P.distance(V.vertex1) < min_distance_to_clipping) {
            return 2;
        }

        return 0;
    }

    return 0;
}

Vertex3D Maths::getCenterVertices(Vertex3D vertices[], int num_vertices) {
    Vertex3D middle = Vertex3D(0, 0, 0);

    for (int i = 0; i < num_vertices; i++) {
        middle.x += vertices[i].x;
        middle.y += vertices[i].y;
        middle.z += vertices[i].z;
        middle.u += vertices[i].u;
        middle.v += vertices[i].v;
    }

    middle.x/= num_vertices;
    middle.y/= num_vertices;
    middle.z/= num_vertices;

    middle.u/= num_vertices;
    middle.v/= num_vertices;

    return middle;
}

void Maths::sortVerticesByY(Vertex3D &A, Vertex3D &B, Vertex3D &C)
{
    int n = 3;
    Vertex3D v[3];
    v[0] = A; v[1] = B; v[2] = C;

    for (int i = 1 ; i < n; i++) {
        for (int j = 0 ; j < (n - i); j++) {
            if (v[j].y > v[j+1].y) {
                Vertex3D aux = v[j];
                v[j] = v[j+1];
                v[j+1] = aux;
            }
        }
    }

    A = v[0]; B = v[1]; C = v[2];

    return;
}

void Maths::sortVerticesByX(Vertex3D &A, Vertex3D &B, Vertex3D &C)
{
    int n = 3;
    Vertex3D v[3];
    v[0] = A; v[1] = B; v[2] = C;

    for (int i = 1 ; i < n; i++) {
        for (int j = 0 ; j < (n - i); j++) {
            if (v[j].x > v[j+1].x) {
                Vertex3D aux = v[j];
                v[j] = v[j+1];
                v[j+1] = aux;
            }
        }
    }

    A = v[0]; B = v[1]; C = v[2];

    return;
}

void Maths::sortPointsByY(Point2D &A, Point2D &B, Point2D &C)
{
    int n = 3;
    Point2D p[3];
    p[0] = A; p[1] = B; p[2] = C;


    for (int i = 1 ; i < n ; i++) {
        for (int j = 0 ; j < (n - i) ; j++) {
            if (p[j].y > p[j+1].y) {
                Point2D aux = p[j];
                p[j] = p[j+1];
                p[j+1] = aux;
            }
        }
    }

    A = p[0]; B = p[1]; C = p[2];

    return;
}

void Maths::sortPointsByX(Point2D &A, Point2D &B, Point2D &C)
{
    int n = 3;
    Point2D p[3];
    p[0] = A; p[1] = B; p[2] = C;

    for (int i = 1 ; i < n ; i++) {
        for (int j = 0 ; j < (n - i) ; j++) {
            if (p[j].x > p[j+1].x) {
                Point2D aux = p[j];
                p[j] = p[j+1];
                p[j+1] = aux;
            }
        }
    }

    A = p[0]; B = p[1]; C = p[2];
}

void Maths::VertexSwap(Vertex3D vertexes[], int i, int j)
{
    Vertex3D tmp = vertexes[i];
    vertexes[i] = vertexes[j];
    vertexes[j] = tmp;
}

void Maths::sortVerticesByX(Vertex3D *vertexes, int N)
{
    int i, j, k;
    for (i = 0; i < N - 1; i++)
    {
        for (k = i, j = i + 1; j < N; j++) {
            if (vertexes[j].x < vertexes[k].x) {
                k = j;
            }
        }
        Maths::VertexSwap (vertexes, i, k);
    }
}

void Maths::sortVerticesByY(Vertex3D *vertexes, int N)
{
    int i, j, k;
    for (i = 0; i < N - 1; i++)
    {
        for (k = i, j = i + 1; j < N; j++) {
            if (vertexes[j].x < vertexes[k].x) {
                k = j;
            }
        }
        Maths::VertexSwap (vertexes, i, k);
    }
}

float Maths::distanteBetweenpoints(Vertex3D v1, Vertex3D v2)
{

    float abs_vector = sqrtf( (v2.x - v1.x)*(v2.x - v1.x) + (v2.y - v1.y)*(v2.y - v1.y) + (v2.z - v1.z)*(v2.z - v1.z) );

    return abs_vector;
}

Uint32 Maths::mixColor(Uint32 color, float distance, LightPoint3D *lp, Vertex3D Q)
{

    Vertex3D P = *lp->getPosition();
    Vertex3D R = lp->AxisForward();

    Vector3D L = Vector3D(P, Q);
    Vertex3D Lv = L.getUnitVector();

    const float min = Vertex3D::dotProduct(R, Lv);

    float p = 100;
    float max = fmaxf(min, 0);
    float pow = powf(max, p);

    float intensity = pow / (lp->kc + lp->kl*distance + lp->kq * (distance * distance));

    int r_light = (int) (Tools::getRedValueFromColor(lp->color)   * intensity);
    int g_light = (int) (Tools::getGreenValueFromColor(lp->color) * intensity);
    int b_light = (int) (Tools::getBlueValueFromColor(lp->color)  * intensity);

    int r_original = (int) (Tools::getRedValueFromColor(color) * ( 1 - intensity) );
    int g_original = (int) (Tools::getGreenValueFromColor(color) * ( 1 - intensity) );
    int b_original = (int) (Tools::getBlueValueFromColor(color) * ( 1 - intensity) );

    Uint32 c = Tools::createRGB(
            r_light + r_original,
            g_light + g_original,
            b_light + b_original
    );

    return c;
}

float Maths::getHorizontalAngleBetweenObject3DAndCamera(Object3D *o1, Camera3D *cam)
{
    Vertex3D oRight = o1->AxisForward();
    Vertex3D R = cam->AxisForward();

    float rads = acosf(  Vertex3D::dotProduct(R, oRight) / (R.getModule() * oRight.getModule()) );

    float degs = Maths::radiansToDegrees(rads);

    float dot2d = oRight.x * R.y - oRight.y * R.x;

    if (signbit(dot2d)) {
        degs =  360 - degs;
    }

    return degs;
}

long Maths::GetNextActive(long x, long vertexCount, const bool *active)
{
    for (;;)
    {
        if (++x == vertexCount) x = 0;
        if (active[x])
            return (x);
    }
}

long Maths::GetPrevActive( long x, long vertexCount, const bool *active)
{
    for (;;) {
        if (--x == -1) x = vertexCount - 1;
        if (active[x])
            return (x);
    }
}

int Maths::TriangulatePolygon(long vertexCount, Vertex3D *vertices, Vertex3D normal, Triangle *triangle, int &ntriangles, Object3D *parent, Texture *texture, bool clipped)
{
    bool *active = new bool[vertexCount];
    for (long a = 0; a < vertexCount; a++) active[a] = true;
    int triangleCount = 0;
    long start = 0;
    long p1 = 0;
    long p2 = 1;
    long m1 = vertexCount - 1;
    long m2 = vertexCount - 2;
    bool lastPositive = false;
    for (;;) {
        if (p2 == m2) {
            // Only three vertices remain.
            // Triangle creation
            Vertex3D tv1 = Transforms::objectToLocal(vertices[m1], parent);
            Vertex3D tv2 = Transforms::objectToLocal(vertices[p1], parent);
            Vertex3D tv3 = Transforms::objectToLocal(vertices[p2], parent);

            Triangle t = Triangle(tv1, tv2, tv3, parent);
            t.setTexture(texture);
            t.setClipped(clipped);
            triangle[ntriangles] = t;
            ntriangles++;

            triangleCount++;
            break;
        }

        Vertex3D vp1 = vertices[p1];
        Vertex3D vp2 = vertices[p2];
        Vertex3D vm1 = vertices[m1];
        Vertex3D vm2 = vertices[m2];

        bool positive = false;
        bool negative = false;

        // Determine whether vp1, vp2, and vm1 form a valid triangle.
        Vertex3D n1 = normal % (vm1 - vp2).getNormalize();
        if (n1 * (vp1 - vp2) > EngineSetup::getInstance()->EPSILON ) {
            positive   = true;
            Vertex3D n2 = (normal % (vp1 - vm1).getNormalize());
            Vertex3D n3 = (normal % (vp2 - vp1).getNormalize());
            for (long a = 0; a < vertexCount; a++) {
                // Look for other vertices inside the triangle.
                if ((active[a]) && (a != p1) && (a != p2) && (a != m1)) {
                    Vertex3D v = vertices[a];
                    if (   (n1 * (v - vp2).getNormalize() > -EngineSetup::getInstance()->EPSILON)
                        && (n2 * (v - vm1).getNormalize() > -EngineSetup::getInstance()->EPSILON)
                        && (n3 * (v - vp1).getNormalize() > -EngineSetup::getInstance()->EPSILON))
                    {
                        positive = false;
                        break;
                    }
                }
            }
        }

        // Determine whether vm1, vm2, and vp1 form a valid triangle.
        n1 = normal % (vm2 - vp1).getNormalize();
        if (n1 * (vm1 - vp1) > EngineSetup::getInstance()->EPSILON) {
            negative = true;
            Vertex3D n2 = (normal % (vm1 - vm2).getNormalize());
            Vertex3D n3 = (normal % (vp1 - vm1).getNormalize());
            for (long a = 0; a < vertexCount; a++)
            {
                // Look for other vertices inside the triangle.
                if ((active[a]) && (a != m1) && (a != m2) && (a != p1)) {
                    Vertex3D v = vertices[a];
                    if (   (n1 * (v - vp1).getNormalize() > -EngineSetup::getInstance()->EPSILON)
                        && (n2 * (v - vm2).getNormalize() > -EngineSetup::getInstance()->EPSILON)
                        && (n3 * (v - vm1).getNormalize() > -EngineSetup::getInstance()->EPSILON))
                    {
                        negative = false;
                        break;
                    }
                }
            }
        }

        // If both triangles are valid, choose the one having
        // the larger smallest angle.
        if ((positive) && (negative)) {
            float pd = (vp2 - vm1).getNormalize() * (vm2 - vm1).getNormalize();
            float md = (vm2 - vp1).getNormalize() * (vp2 - vp1).getNormalize();
            if (fabs(pd - md) < EngineSetup::getInstance()->EPSILON) {
                if (lastPositive) positive = false;
                else negative = false;
            } else {
                if (pd < md) negative = false;
                else positive = false;
            }
        }

        if (positive) {
            // Output the triangle m1, p1, p2.
            active[p1] = false;

            Vertex3D tv1 = Transforms::objectToLocal(vertices[m1], parent);
            Vertex3D tv2 = Transforms::objectToLocal(vertices[p1], parent);
            Vertex3D tv3 = Transforms::objectToLocal(vertices[p2], parent);

            Triangle t = Triangle(tv1, tv2, tv3, parent);
            t.setTexture(texture);
            t.setClipped(clipped);

            triangle[ntriangles] = t;
            ntriangles++;

            triangleCount++;
            //triangle++;
            p1 = GetNextActive(p1, vertexCount, active);
            p2 = GetNextActive(p2, vertexCount, active);
            lastPositive = true;
            start =  -1;
        } else if (negative) {
            // Output the triangle m2, m1, p1.
            active[m1] = false;

            Vertex3D tv1 = Transforms::objectToLocal(vertices[m2], parent);
            Vertex3D tv2 = Transforms::objectToLocal(vertices[m1], parent);
            Vertex3D tv3 = Transforms::objectToLocal(vertices[p1], parent);

            Triangle t = Triangle(tv1, tv2, tv3, parent);
            t.setTexture(texture);
            t.setClipped(clipped);

            triangle[ntriangles] = t;
            ntriangles++;

            triangleCount++;
            //triangle++;
            m1 = GetPrevActive(m1, vertexCount, active);
            m2 = GetPrevActive(m2, vertexCount, active);
            lastPositive = false;
            start = -1;
        } else {
            // Exit if we've gone all the way around the
            // polygon without finding a valid triangle.
            if (start == -1) start = p2;
            else if (p2 == start) {
                break;
            }

            // Advance working set of vertices.
            m2 = m1;
            m1 = p1;
            p1 = p2;
            p2 = GetNextActive(p2, vertexCount, active);
        }
    }

    delete[] active;
    return (triangleCount);
}

bool Maths::ClippingPolygon(Vertex3D *input, int ninput, Vertex3D *output, int &noutput, int id_plane, Camera3D *cam)
{
    Vector3D edge;

    bool new_vertices = false;

    //Logging::getInstance()->Log(">>>> ClippingPolygon (" + std::to_string(ninput)+")", "");

    //std::string lo = "";

    for (int i = 0; i < ninput; i++) {
        int next = i + 1;
        if ( next < ninput ) {
            edge = Vector3D(input[i], input[next]);
        } else {
            edge = Vector3D(input[i], input[0]);
        }
        // test clip plane
        int testClip = Maths::isVector3DClippingPlane( cam->frustum->planes[ id_plane ], edge );

        //Logging::getInstance()->Log("Clipping against plane " + std::to_string(id_plane) + " | testClip: " + std::to_string(testClip), "");

        /** 0 = dos vértices dentro | 1 = ningún vértice dentro | 2 = vértice A dentro | 3 = vértice B dentro */
        // Si el primer vértice está dentro, lo añadimos a la salida
        if (testClip == 0 || testClip == 2) {
            //lo+="First(u:"+std::to_string(edge.vertex1.u)+", v:"+std::to_string(edge.vertex1.v)+")-";
            output[noutput] = edge.vertex1; noutput++;
        }

        // Si el primer y el segundo vértice no tienen el mismo estado añadimos el punto de intersección al plano
        if (testClip > 1) {
            float t = 0;
            Vertex3D newVertex = cam->frustum->planes[id_plane].getPointIntersection(edge.vertex1, edge.vertex2, t);
            newVertex.u = edge.vertex1.u + t * (edge.vertex2.u - edge.vertex1.u);
            newVertex.v = edge.vertex1.v + t * (edge.vertex2.v - edge.vertex1.v);
            //lo+="New(u:"+std::to_string(newVertex.u)+", v:"+std::to_string(newVertex.v)+")-";

            output[noutput] = newVertex; noutput++;
            new_vertices = true;
        }
    }

    //Logging::getInstance()->Log("= Vertices after clipping: " + lo, "");
    //Logging::getInstance()->Log("", "");

    return new_vertices;
}
