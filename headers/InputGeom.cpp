#include "InputGeom.h"

InputGeom::InputGeom() {

}

void InputGeom::loadFromMesh3D( Triangle *model_triangles, int n_triangles )
{
    m_mesh = new rcMeshLoaderObj;

    int cap = 0;
    int tcap = 0;

    for (int i = 0; i < n_triangles; i++) {
        int baseIndex = i*3;
        m_mesh->addVertex(model_triangles[i].A.x, model_triangles[i].A.y, model_triangles[i].A.z, cap);
        m_mesh->addVertex(model_triangles[i].B.x, model_triangles[i].B.y, model_triangles[i].B.z, cap);
        m_mesh->addVertex(model_triangles[i].C.x, model_triangles[i].C.y, model_triangles[i].C.z, cap);
        m_mesh->addTriangle(baseIndex, baseIndex+1, baseIndex+2, tcap);
    }

    rcCalcBounds(m_mesh->getVerts(), m_mesh->getVertCount(), m_meshBMin, m_meshBMax);

}