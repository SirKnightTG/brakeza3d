
#ifndef SDL2_3D_ENGINE_PLANE_H
#define SDL2_3D_ENGINE_PLANE_H


#include "../Objects/Vertex3D.h"
#include "../Objects/Vector3D.h"

class Plane {
public:
    Vertex3D A; // origin
    Vertex3D B;
    Vertex3D C;

    Vertex3D normal;

    Plane(const Vertex3D A, const Vertex3D B, const Vertex3D C);
    Plane(const Vertex3D P, const Vertex3D N);
    Plane();

    float distance(const Vertex3D &p);

    Vertex3D getNormalVector();
    void     updateNormalVector();

    Vertex3D getPointIntersection(Vertex3D v1, Vertex3D v2, float &transition);
    bool isFrontFacingTo(Vertex3D direction);
    bool intersect(Vector3D ray, float &t);

    void setOrigin(Vertex3D);
    void setNormal(Vertex3D);

    Vertex3D origin() const;
    Vertex3D closest( Vertex3D p);

};


#endif //SDL2_3D_ENGINE_PLANE_H
