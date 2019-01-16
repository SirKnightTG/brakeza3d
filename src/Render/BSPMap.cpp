
#include <fcntl.h> // open
#include <stdio.h> // printf
#include <stdlib.h> // malloc, free
#include <sys/stat.h> // stat
#include <unistd.h> // lseek, read, close
#include <cstring>
#include "../../headers/Render/BSPMap.h"
#include "../../headers/Objects/Vertex3D.h"
#include "../../headers/Render/Drawable.h"
#include "../../headers/Render/Maths.h"
#include "../../headers/Render/Transforms.h"
#include "../../headers/Render/EngineBuffers.h"
#include "../../headers/Render/Logging.h"
#include "../../headers/Render/Engine.h"

BSPMap::BSPMap()
{
    bsp = NULL;
    surfacePrimitives = NULL;
    textureObjNames = NULL;
    visibleSurfaces = NULL;

    if (surfacePrimitives) delete[] surfacePrimitives;
    if (textureObjNames) delete[] textureObjNames;
    if (visibleSurfaces) delete[] visibleSurfaces;

    n_triangles = 0;
    this->model_triangles = new Triangle[MAX_BSP_TRIANGLES];
    this->textures = new Texture[MAX_MAP_TEXTURES];

    this->demo_texture = new Texture();
    this->demo_texture->loadBMP("../textures/chessboard.bmp");

    // La posición de un mapa ha de ser fija ya que la información cacheada está referida a esta referencia
    // La rotación x = 180, y = 90, es para ajustar los ejes con los que quake1 trabaja. (z la vertical)
    setPosition( Vertex3D(0, 0, 0) );
    setRotation( M3(180, 90, 0) );

    this->surface_triangles = new surface_triangles_t[MAX_BSP_TRIANGLES];
}

bool BSPMap::Initialize(const char *bspFilename, const char *paletteFilename)
{
    std::string bspFilename_str = std::string(EngineSetup::getInstance()->ASSETS_FOLDER + bspFilename).c_str();
    std::string paletteFilename_str = std::string(EngineSetup::getInstance()->ASSETS_FOLDER + paletteFilename).c_str();

    if (!LoadBSP(bspFilename_str.c_str())) {
        printf("[ERROR] Map::Initialize() Error loading bsp file\n");
        return false;
    }

    if (!LoadPalette(paletteFilename_str.c_str())) {
        printf("[ERROR] Map::Initialize() Error loading palette\n");
        return false;
    }

    return true;
}

int BSPMap::LoadFile(const char *filename, void **bufferptr)
{
    struct stat st;
    void *buffer;
    int length;

    int f = open(filename, O_RDONLY);
    if (f == -1) {
        return 0;
    }

    stat(filename, &st);
    length = st.st_size;
    lseek(f, 0, SEEK_SET);
    buffer = malloc(length);
    read(f, buffer, length);
    close(f);

    *bufferptr = buffer;
    return length;
}

bool BSPMap::LoadPalette(const char *filename)
{
    unsigned char *tempPal;

    if (!LoadFile(filename, (void **)&tempPal)) {
        return false;
    }

    for (int i = 0; i < 256; i++) {
        unsigned int b = tempPal[i * 3 + 0];
        unsigned int g = tempPal[i * 3 + 1];
        unsigned int r = tempPal[i * 3 + 2];
        palette[i] = (r) | (g << 8) | (b << 16);
    }

    free(tempPal);
    return true;
}

bool BSPMap::LoadBSP(const char *filename)
{
    if (!LoadFile(filename, (void **)&bsp)) {
        return false;
    }

    header = (dheader_t *)bsp;
    if (header->version != BSP_VERSION) {
        printf("[ERROR] Map::LoadBSP() BSP file version mismatch!\n");
        return false;
    }

    return true;
}

// Calculate primitive surfaces
bool BSPMap::InitializeSurfaces(void)
{
    // Allocate memory for the visible surfaces array
    visibleSurfaces = new int [this->getNumSurfaceLists()];

    // Calculate max number of edges per surface
    numMaxEdgesPerSurface = 0;
    for (int i = 0; i < this->getNumSurfaces(); i++) {
        if (numMaxEdgesPerSurface < this->getNumEdges(i)) {
            numMaxEdgesPerSurface = this->getNumEdges(i);
        }
    }

    // Allocate memory for the surface primitive array
    surfacePrimitives = new primdesc_t [this->getNumSurfaces() * numMaxEdgesPerSurface];

    // Loop through all the surfaces to fetch the vertices and calculate it's texture coords
    for (int i = 0; i < this->getNumSurfaces(); i++) {
        int numEdges = this->getNumEdges(i);

        // Get a pointer to texinfo for this surface
        texinfo_t *textureInfo = this->getTextureInfo(i);
        // Get a pointer to the surface's miptextures
        miptex_t *mipTexture = this->getMipTexture(textureInfo->texid);

        // Point to a surface primitive array
        primdesc_t *primitives = &surfacePrimitives[i * numMaxEdgesPerSurface];

        for (int j = 0; j < numEdges; j++, primitives++) {
            // Get an edge id from the surface. Fetch the correct edge by using the id in the Edge List.
            // The winding is backwards!
            int edgeId = this->getEdgeList(this->getSurface(i)->firstedge + (numEdges - 1 - j));

            // Fetch one of the egde's vertex
            int vertexId = ((edgeId >= 0) ? this->getEdge(edgeId)->startVertex : this->getEdge(-edgeId)->endVertex);

            // Store the vertex in the primitive array
            vec3_t *vertex = this->getVertex(vertexId);
            primitives->v[0] = ((float *)vertex)[0];
            primitives->v[1] = ((float *)vertex)[1];
            primitives->v[2] = ((float *)vertex)[2];

            // Calculate the vertex's texture coords and store it in the primitive array
            primitives->t[1] = (CalculateDistance(textureInfo->snrm, primitives->v) + textureInfo->soff) / mipTexture->width;
            primitives->t[0] = (CalculateDistance(textureInfo->tnrm, primitives->v) + textureInfo->toff) / mipTexture->height;

        }
    }

    return true;
}

// Build mipmaps from the textures, create texture objects and assign mipmaps to the created objects
bool BSPMap::InitializeTextures(void)
{
    int numAnimTextues = 0;
    int numSkyTextures = 0;

    // Loop through all textures to count animations and sky textures
    for (int i = 0; i < this->getNumTextures(); i++) {
        // Point to the stored mipmaps
        miptex_t *mipTexture = this->getMipTexture(i);
        if (strncmp(mipTexture->name, "*", 1) == 0) {
            numAnimTextues++;
        } else if (strncmp(mipTexture->name, "sky", 3) == 0) {
            numSkyTextures++;
        }
    }
    //printf("%d animated textures, %d sky textures\n", numAnimTextues, numSkyTextures);

    // Calculate the total number of textures
    int numTotalTextures = this->getNumTextures() + numAnimTextues * (ANIM_TEX_FRAMES - 1) + numSkyTextures * 2;

    // Allocate memory for texture names and an array to store the textures we are using
    textureObjNames = new unsigned int [numTotalTextures];
    textures = new Texture[numTotalTextures];


    // Loop through all texture objects to associate them with a texture and calculate mipmaps from it
    for (int i = 0; i < this->getNumTextures(); i++) {
        // Point to the stored mipmaps
        miptex_t *mipTexture = this->getMipTexture(i);

        // NULL textures exist, don't use them!
        if (!mipTexture->name[0]) {
            continue;
        }

        int width = mipTexture->width;
        int height = mipTexture->height;

        // Allocate memory for the texture which will be created
        unsigned int *texture = new unsigned int [width * height];

        // Point to the raw 8-bit texture data
        unsigned char *rawTexture = this->getRawTexture(i);

        textures[i] = Texture(width, height);

        // Create a texture to assign with the texture object
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                unsigned int color = this->palette[rawTexture[x + y * width]];
                texture[x + y * width] = color;
            }
        }

        textures[i].loadFromRaw(texture, width, height);
        textures[i].setFilename( std::string(mipTexture->name) );

        Logging::getInstance()->Log(
            "Loading Q1Map Texture idx: " + std::to_string(i) + " '" + textures[i].getFilename().c_str() + "'" +
            " | w: " + std::to_string(textures[i].getSurface()->w) +
            " | h: " + std::to_string(textures[i].getSurface()->h),
            "Q1MAP"
        );

        delete texture;
    }

    return true;
}

bool BSPMap::InitializeTriangles(Camera3D *cam)
{
    for (int i = 0; i < this->getNumSurfaces(); i++) {
        // Creamos triángulos
        int start_number_triangle = this->n_triangles;
        this->createTrianglesForSurface(i, cam);
        int end_number_triangle = this->n_triangles;
        int surface_num_triangles = end_number_triangle - start_number_triangle ;

        // Creamos el registro que relaciona una surface con sus triángulos
        this->surface_triangles[i].num = surface_num_triangles;
        this->surface_triangles[i].offset = start_number_triangle;

        /*Logging::getInstance()->Log(
            "Surface: " + std::to_string(i) + " - triangles:  "+ std::to_string(surface_num_triangles) +" | offset: " +
            std::to_string(start_number_triangle) + ", ends: " +
            std::to_string(end_number_triangle) + ", Total triangles: " +
            std::to_string(n_triangles)
            , ""
        );*/
    }

    for (int i = 0; i < n_triangles; i++){
        this->model_triangles[i].is_bsp = true;
    }
}

void BSPMap::createTrianglesForSurface(int surface, Camera3D *cam)
{
    // Get the surface primitive
    primdesc_t *primitives = &surfacePrimitives[numMaxEdgesPerSurface * surface];

    Vertex3D vertices[30];  // Max face vertex
    int num_vertices = 0;

    // Loop through all vertices of the primitive and draw a surface
    int num_edges = this->getNumEdges(surface);

    for (int i = 0; i < num_edges ; i++, primitives++) {
        // IMPORTANTE: Las coordenadas del mapa de quake1 son distintas (la Z es la vertical) por eso usamos el orden (y, z, x)
        Vertex3D v = Vertex3D(
            primitives->v[1],
            primitives->v[2],
            primitives->v[0]
        );

        v.u = primitives->t[0];
        v.v = primitives->t[1];

        v = Transforms::objectToLocal(v, this);

        vertices[num_vertices] = v;
        num_vertices++;
    }

    triangulateQuakeSurface(vertices, num_vertices, surface);
}

// Return the dotproduct of two vectors
float BSPMap::CalculateDistance(vec3_t a, vec3_t b)
{
    return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
}

void BSPMap::DrawSurfaceTriangles(int surface, Camera3D *cam)
{
    const int offset = this->surface_triangles[surface].offset;
    const int num = this->surface_triangles[surface].num;

    for (int i = offset; i < offset+num; i++){
        this->model_triangles[i].draw(cam);
    }
}

bool BSPMap::triangulateQuakeSurface(Vertex3D vertices[], int num_vertices, int surface)
{
    Vertex3D normal;

    for (int i = 0, j = 1; i<num_vertices; i++, j++) {
        if (j == num_vertices) j = 0;
        normal.x += (((vertices[i].z) + (vertices[j].z)) * ((vertices[j].y) - (vertices[i].y)));
        normal.y += (((vertices[i].x) + (vertices[j].x)) * ((vertices[j].z) - (vertices[i].z)));
        normal.z += (((vertices[i].y) + (vertices[j].y)) * ((vertices[j].x) - (vertices[i].x)));
    }
    normal = normal.getInverse();

    texinfo_t *textureInfo = this->getTextureInfo(surface);

    Maths::TriangulatePolygon(
        num_vertices,
        vertices,
        normal,
        this->model_triangles,
        this->n_triangles,
        this,
        &textures[textureInfo->texid],
        false
    );

    return true;
}

// Calculate which other leaves are visible from the specified leaf, fetch the associated surfaces and draw them
void BSPMap::DrawLeafVisibleSet(bspleaf_t *pLeaf, Camera3D *cam)
{
    int numVisibleSurfaces = 0;

    // Get a pointer to the visibility list that is associated with the BSP leaf
    unsigned char * visibilityList = this->getVisibilityList(pLeaf->vislist);

    for ( int i = 1; i < this->getNumLeaves(); visibilityList++) {
        const unsigned char veces = *(visibilityList);
        if (veces == 0) {
            i += 8 * *(++visibilityList);
        } else {
            for (int j = 0; j < 8; j++, i++) {
                if (Tools::getBit(veces, j)) {
                    bspleaf_t *visibleLeaf = this->getLeaf(i);
                    int firstSurface = visibleLeaf->firstsurf;
                    int lastSurface = firstSurface + visibleLeaf->numsurf;
                    for (int k = firstSurface; k < lastSurface; k++) {
                        visibleSurfaces[numVisibleSurfaces++] = this->getSurfaceList(k);
                    }
                }
            }
        }
    }

    DrawSurfaceList(visibleSurfaces, numVisibleSurfaces, cam);
}


// Calculate which other leaves are visible from the specified leaf, fetch the associated surfaces and draw them
void BSPMap::PhysicsLeafVisibleSet(bspleaf_t *pLeaf, Camera3D *cam)
{
    int numVisibleSurfaces = 0;

    // Get a pointer to the visibility list that is associated with the BSP leaf
    unsigned char * visibilityList = this->getVisibilityList(pLeaf->vislist);

    for ( int i = 1; i < this->getNumLeaves(); visibilityList++) {
        unsigned char veces = *(visibilityList);
        if (veces == 0) {
            i += 8 * *(++visibilityList);
        } else {
            for (int j = 0; j < 8; j++, i++) {
                if (Tools::getBit(veces, j)) {
                    bspleaf_t *visibleLeaf = this->getLeaf(i);
                    int firstSurface = visibleLeaf->firstsurf;
                    int lastSurface = firstSurface + visibleLeaf->numsurf;
                    for (int k = firstSurface; k < lastSurface; k++) {
                        visibleSurfaces[numVisibleSurfaces++] = this->getSurfaceList(k);
                    }
                }
            }
        }
    }

    for (int i = 0; i < numVisibleSurfaces; i++) {

        int surface = visibleSurfaces[i];

        const int offset = this->surface_triangles[surface].offset;
        const int num = this->surface_triangles[surface].num;

        for (int i = offset; i < offset+num; i++) {
            if (model_triangles[i].isCollisionWithSphere(cam->collider, EngineSetup::getInstance()->PLAYER_SPHERE_RADIUS, cam)) {
            }
        }
    }
}

// Draw the visible surfaces
void BSPMap::DrawSurfaceList(int *visibleSurfaces, int numVisibleSurfaces, Camera3D *cam)
{
    if (EngineSetup::getInstance()->DEBUG_BSP_MODE) {
        EngineBuffers::getInstance()->resetBenchmarkValues();
    }

    // Loop through all the visible surfaces and activate the texture objects
    for (int i = 0; i < numVisibleSurfaces; i++) {
        DrawSurfaceTriangles(visibleSurfaces[i], cam);
    }
}

// Traverse the BSP tree to find the leaf containing visible surfaces from a specific position
bspleaf_t *BSPMap::FindLeaf(Camera3D *camera)
{
    bspleaf_t *leaf = NULL;

    // Fetch the start node
    bspnode_t *node = this->getStartNode();

    while (!leaf) {
        short nextNodeId;

        // Get a pointer to the plane which intersects the node
        plane_t *plane = this->getPlane(node->planenum);

        // Calculate distance to the intersecting plane
        Vertex3D cp = *camera->getPosition();
        cp = M3(-90, 0, 0) * cp;

        vec3_t cam_pos;
        cam_pos[0] = cp.x;
        cam_pos[1] = cp.y;
        cam_pos[2] = cp.z;

        float distance = CalculateDistance(plane->normal, cam_pos);

        // If the camera is in front of the plane, traverse the right (front) node, otherwise traverse the left (back) node
        if (distance > plane->dist) {
            nextNodeId = node->front;
        } else {
            nextNodeId = node->back;
        }

        // If next node >= 0, traverse the node, otherwise use the inverse of the node as the index to the leaf (and we are done!)
        if (nextNodeId >= 0) {
            node = this->getNode(nextNodeId);
        } else {
            leaf = this->getLeaf(~nextNodeId);
        }
    }

    return leaf;
}