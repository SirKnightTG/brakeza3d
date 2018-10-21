
#ifndef SDL2_3D_ENGINE_MESH_H
#define SDL2_3D_ENGINE_MESH_H

#include <string>
#include "Vertex3D.h"
#include "Triangle3D.h"
#include "../Render/Tools.h"
#include "../Q3MapLoader/Q3Loader.h"
#include "../Render/EngineSetup.h"
#include "Object3D.h"
#include "LightPoint3D.h"

#define MAX_MODEL_TRIANGLES 10000
#define MAX_VERTEX_MODEL 100000
#define MAX_MESH_TEXTURES 100

class Mesh3D : public Object3D {

public:
    TMapQ3 lMap;
    Billboard *billboard;

    std::string mesh_file;

    Texture *model_textures;
    Triangle *model_triangles;
    Vertex3D *model_vertex;

    int n_triangles;
    int n_vertex;
    int n_textures;

    LightPoint3D **lightPoints;
    int numberLightPoints;

    bool shadowCaster;

    bool isShadowCaster() const;
    void setShadowCaster(bool shadow_caster);

    Mesh3D();

    // OBJ
    bool loadOBJBlender(const char *name);
    void loadOBJBlenderVertex();
    void loadOBJBlenderTriangles();
    void loadOBJBlenderTextureCoordinates();
    void loadOBJBlenderMaterials();

    // BSP
    void loadQ3Map(const std::string &pFilename);
    void loadTrianglesFromQ3Map();
    void loadTexturesFromQ3Map();

    void draw(Camera3D *);
    void shadowMapping(LightPoint3D *);
    void setLightPoints(LightPoint3D **lightPoint, int);
};


#endif //SDL2_3D_ENGINE_MESH_H