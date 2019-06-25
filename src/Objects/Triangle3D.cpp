
#include <SDL_surface.h>
#include <cmath>
#include <list>
#include "../../headers/Objects/Triangle3D.h"
#include "../../headers/Render/Color.h"
#include "../../headers/Render/Tools.h"
#include "../../headers/Render/EngineSetup.h"
#include "../../headers/Objects/Line2D.h"
#include "../../headers/Render/EngineBuffers.h"
#include "../../headers/Render/Transforms.h"
#include "../../headers/Render/Drawable.h"
#include "../../headers/Render/M3.h"
#include "../../headers/Render/Engine.h"
#include "../../headers/Render/Logging.h"
#include "../../headers/Render/Maths.h"

extern Engine *brakeza3D;

Triangle::Triangle()
{
    texture = NULL;
    parent = NULL;
    lightmap = new Texture();
}

Triangle::Triangle(Vertex3D A, Vertex3D B, Vertex3D C, Object3D *parent)
{
    this->A = A;
    this->B = B;
    this->C = C;

    this->parent = parent;

    // la textura se carga bajo demanda
    texture = NULL;
    this->lod = 1;

    lightmap = new Texture();
}

void Triangle::updateObjectSpace()
{
    Ao = Transforms::objectSpace(A, parent);
    Bo = Transforms::objectSpace(B, parent);
    Co = Transforms::objectSpace(C, parent);
}

void Triangle::updateCameraSpace(Camera3D *cam)
{
    Ac = Transforms::cameraSpace(Ao, cam);
    Bc = Transforms::cameraSpace(Bo, cam);
    Cc = Transforms::cameraSpace(Co, cam);
}

void Triangle::updateNDCSpace(Camera3D *cam)
{
    An = Transforms::NDCSpace(Ac, cam);
    Bn = Transforms::NDCSpace(Bc, cam);
    Cn = Transforms::NDCSpace(Cc, cam);
}

void Triangle::updateScreenSpace(Camera3D *cam)
{
    As = Transforms::screenSpace(An, cam);
    Bs = Transforms::screenSpace(Bn, cam);
    Cs = Transforms::screenSpace(Cn, cam);
}

void Triangle::updateFullVertexSpaces(Camera3D *cam)
{
    this->updateObjectSpace();
    this->updateCameraSpace(cam);
    this->updateNDCSpace(cam);
    this->updateScreenSpace(cam);
    this->updateNormal();
}

void Triangle::updateUVCache()
{
    // lightmap coordinates
    if (getLightmap()->isLightMapped()) {
        light_u1 = Ac.u;
        light_v1 = Ac.v;

        light_u2 = Bc.u;
        light_v2 = Bc.v;

        light_u3 = Cc.u;
        light_v3 = Cc.v;
    }

    // texture coordinates
    if (this->getTexture() != NULL) {
        if (is_bsp) {
            tex_u1 = Ac.u / getTexture()->getSurface(1)->w;
            tex_v1 = Ac.v / getTexture()->getSurface(1)->h;

            tex_u2 = Bc.u / getTexture()->getSurface(1)->w;
            tex_v2 = Bc.v / getTexture()->getSurface(1)->h;

            tex_u3 = Cc.u / getTexture()->getSurface(1)->w;
            tex_v3 = Cc.v / getTexture()->getSurface(1)->h;
        } else {
            tex_u1 = Ac.u;
            tex_v1 = Ac.v;

            tex_u2 = Bc.u;
            tex_v2 = Bc.v;

            tex_u3 = Cc.u;
            tex_v3 = Cc.v;
        }

        light_u1_Ac_z = light_u1 / Ac.z;
        light_u2_Bc_z = light_u2 / Bc.z;
        light_u3_Cc_z = light_u3 / Cc.z;

        light_v1_Ac_z = light_v1 / Ac.z;
        light_v2_Bc_z = light_v2 / Bc.z;
        light_v3_Cc_z = light_v3 / Cc.z;

        tex_u1_Ac_z = tex_u1 / Ac.z;
        tex_u2_Bc_z = tex_u2 / Bc.z;
        tex_u3_Cc_z = tex_u3 / Cc.z;

        tex_v1_Ac_z = tex_v1 / Ac.z;
        tex_v2_Bc_z = tex_v2 / Bc.z;
        tex_v3_Cc_z = tex_v3 / Cc.z;

        persp_correct_Az = 1/Ac.z;
        persp_correct_Bz = 1/Bc.z;
        persp_correct_Cz = 1/Cc.z;
    }
}

void Triangle::updateNormal()
{
    Vector3D v1 = Vector3D(this->Ao, this->Bo);
    Vector3D v2 = Vector3D(this->Ao, this->Co);

    this->normal = v1.getComponent() % v2.getComponent();
}

Vertex3D Triangle::getNormal()
{
    return this->normal;
}

void Triangle::shadowMapping(LightPoint3D *lp)
{
    this->updateFullVertexSpaces(lp->cam);

    if (this->isBackFaceCulling(lp->cam))  {
        return;
    }

    if ( !lp->cam->frustum->isPointInFrustum(Ao) &&
         !lp->cam->frustum->isPointInFrustum(Bo) &&
         !lp->cam->frustum->isPointInFrustum(Co)
            ) {
        return;
    }

    this->scanVerticesForShadowMapping(lp);
}

void Triangle::drawForTile(Camera3D *cam, int minX, int minY, int maxX, int maxY)
{
    softwareRasterizerForTile( minX,  minY,  maxX,  maxY);
}

void Triangle::draw(Camera3D *cam)
{
    drawed = true;
    // rasterization
    if (EngineSetup::getInstance()->TRIANGLE_MODE_TEXTURIZED || EngineSetup::getInstance()->TRIANGLE_MODE_COLOR_SOLID) {
        this->softwareRasterizer();
    }

    // wireframe
    if (EngineSetup::getInstance()->TRIANGLE_MODE_WIREFRAME) {
        this->drawWireframe();
    }

    // Pixels
    if (EngineSetup::getInstance()->TRIANGLE_MODE_PIXELS ) {
        Drawable::drawVertex(Co, cam, Color::red());
        Drawable::drawVertex(Bo, cam, Color::green());
        Drawable::drawVertex(Co, cam, Color::blue());
    }

    EngineBuffers::getInstance()->trianglesDrawed++;
}

bool Triangle::clipping(Camera3D *cam, Triangle *arrayTriangles, int &numTriangles)
{
    Vertex3D output_vertices[10] ; int num_outvertices   = 0;
    Vertex3D input_vertices[10]  ; int num_inputvertices = 0;

    input_vertices[0] = this->Ao; num_inputvertices++;
    input_vertices[1] = this->Bo; num_inputvertices++;
    input_vertices[2] = this->Co; num_inputvertices++;

    const int plane_init = EngineSetup::getInstance()->LEFT_PLANE;
    const int plane_end  = EngineSetup::getInstance()->BOTTOM_PLANE;

    // clip against planes
    for (int i = plane_init ; i <= plane_end ; i++) {
        Maths::ClippingPolygon(input_vertices, num_inputvertices, output_vertices, num_outvertices, i, cam);

        memcpy (&input_vertices, &output_vertices, sizeof(output_vertices));
        /*for (int j = 0; j < num_outvertices; j++) { input_vertices[j] = output_vertices[j]; }*/

        num_inputvertices = num_outvertices;
        num_outvertices = 0;
    }

    if (num_inputvertices != 0) {
        int oldNumTriangles = numTriangles;

        Maths::TriangulatePolygon(
                num_inputvertices, input_vertices,
                this->getNormal(),
                arrayTriangles, numTriangles,
                parent,
                this->getTexture(),
                this->getLightmap(),
                true,
                this->is_bsp
        );

        // update cache for clipped triangles (they are out from hide removal surface updating)
        for (int i = oldNumTriangles; i < numTriangles; i++) {
            arrayTriangles[i].updateFullVertexSpaces(cam);
            arrayTriangles[i].updateUVCache();
            arrayTriangles[i].updateBoundingBox();
            arrayTriangles[i].updateFullArea();
        }

        return true;
    }

    return false;
}

void Triangle::softwareRasterizerForTile(int minTileX, int minTileY, int maxTileX, int maxTileY)
{
    // LOD determination
    this->lod = processLOD();

    // Triangle setup
    int A01 = (int) -(As.y - Bs.y);
    int A12 = (int) -(Bs.y - Cs.y);
    int A20 = (int) -(Cs.y - As.y);

    int B01 = (int) -(Bs.x - As.x);
    int B12 = (int) -(Cs.x - Bs.x);
    int B20 = (int) -(As.x - Cs.x);

    Point2D startP(minX, minY);
    int w0_row = Maths::orient2d(Bs, Cs, startP);
    int w1_row = Maths::orient2d(Cs, As, startP);
    int w2_row = Maths::orient2d(As, Bs, startP);

    float alpha, theta, gamma, depth, affine_uv, texu, texv, lightu, lightv;

    for (int y = minY ; y < maxY ; y++) {
        int w0 = w0_row;
        int w1 = w1_row;
        int w2 = w2_row;

        for (int x = minX ; x < maxX ; x++) {

            if ((w0 | w1 | w2) > 0) {

                alpha = w0 * reciprocalFullArea;
                theta = w1 * reciprocalFullArea;
                gamma = 1 - alpha - theta;

                depth = alpha * (An.z) + theta * (Bn.z) + gamma * (Cn.z);

                const int bufferIndex = ( y * EngineSetup::getInstance()->SCREEN_WIDTH ) + x;

                if (EngineSetup::getInstance()->TRIANGLE_RENDER_DEPTH_BUFFER && depth < EngineBuffers::getInstance()->getDepthBuffer( bufferIndex )) {
                    affine_uv = 1 / ( alpha * (persp_correct_Az) + theta * (persp_correct_Bz) + gamma * (persp_correct_Cz) );
                    texu   = ( alpha * (tex_u1_Ac_z)   + theta * (tex_u2_Bc_z)   + gamma * (tex_u3_Cc_z) )   * affine_uv;
                    texv   = ( alpha * (tex_v1_Ac_z)   + theta * (tex_v2_Bc_z)   + gamma * (tex_v3_Cc_z) )   * affine_uv;

                    lightu = ( alpha * (light_u1_Ac_z) + theta * (light_u2_Bc_z) + gamma * (light_u3_Cc_z) ) * affine_uv;
                    lightv = ( alpha * (light_v1_Ac_z) + theta * (light_v2_Bc_z) + gamma * (light_v3_Cc_z) ) * affine_uv;

                    if (! ((x < minTileX || x > maxTileX) || (y < minTileY || y > maxTileY )) ) {
                        processPixel(
                                bufferIndex,
                                x, y,
                                alpha, theta, gamma,
                                depth,
                                texu, texv,
                                lightu, lightv
                        );
                    }
                }
            }

            // edge function increments
            w0 += A12;
            w1 += A20;
            w2 += A01;
        }

        w0_row += B12;
        w1_row += B20;
        w2_row += B01;
    }

}

void Triangle::softwareRasterizer()
{
    // LOD determination
    this->lod = processLOD();

    // Triangle setup
    int A01 = (int) -(As.y - Bs.y);
    int A12 = (int) -(Bs.y - Cs.y);
    int A20 = (int) -(Cs.y - As.y);

    int B01 = (int) -(Bs.x - As.x);
    int B12 = (int) -(Cs.x - Bs.x);
    int B20 = (int) -(As.x - Cs.x);

    Point2D startP(minX, minY);
    int w0_row = Maths::orient2d(Bs, Cs, startP);
    int w1_row = Maths::orient2d(Cs, As, startP);
    int w2_row = Maths::orient2d(As, Bs, startP);

    float alpha, theta, gamma, depth, affine_uv, texu, texv, lightu, lightv;

    for (int y = minY ; y < maxY ; y++) {
        int w0 = w0_row;
        int w1 = w1_row;
        int w2 = w2_row;

        for (int x = minX ; x < maxX ; x++) {

            if ((w0 | w1 | w2) > 0) {

                alpha = w0 * reciprocalFullArea;
                theta = w1 * reciprocalFullArea;
                gamma = 1 - alpha - theta;

                depth = alpha * (An.z) + theta * (Bn.z) + gamma * (Cn.z);

                int bufferIndex = ( y * EngineSetup::getInstance()->SCREEN_WIDTH ) + x;

                if (EngineSetup::getInstance()->TRIANGLE_RENDER_DEPTH_BUFFER && depth < EngineBuffers::getInstance()->getDepthBuffer( bufferIndex )) {
                    affine_uv = 1 / ( alpha * (persp_correct_Az) + theta * (persp_correct_Bz) + gamma * (persp_correct_Cz) );
                    texu   = ( alpha * (tex_u1_Ac_z)   + theta * (tex_u2_Bc_z)   + gamma * (tex_u3_Cc_z) )   * affine_uv;
                    texv   = ( alpha * (tex_v1_Ac_z)   + theta * (tex_v2_Bc_z)   + gamma * (tex_v3_Cc_z) )   * affine_uv;

                    lightu = ( alpha * (light_u1_Ac_z) + theta * (light_u2_Bc_z) + gamma * (light_u3_Cc_z) ) * affine_uv;
                    lightv = ( alpha * (light_v1_Ac_z) + theta * (light_v2_Bc_z) + gamma * (light_v3_Cc_z) ) * affine_uv;

                    processPixel(
                            bufferIndex,
                            x, y,
                            alpha, theta, gamma,
                            depth,
                            texu, texv,
                            lightu, lightv
                    );
                }
            }

            // edge function increments
            w0 += A12;
            w1 += A20;
            w2 += A01;
        }

        w0_row += B12;
        w1_row += B20;
        w2_row += B01;
    }

}

void Triangle::updateBoundingBox()
{
    maxX = std::max(As.x, std::max(Bs.x, Cs.x));
    minX = std::min(As.x, std::min(Bs.x, Cs.x));
    maxY = std::max(As.y, std::max(Bs.y, Cs.y));
    minY = std::min(As.y, std::min(Bs.y, Cs.y));
}

float Triangle::updateFullArea()
{
    this->fullArea = Maths::orient2d(Bs, Cs, Point2D((int) As.x, (int) As.y));
    this->reciprocalFullArea = 1 / this->fullArea;

}

void Triangle::drawWireframe()
{
    Drawable::drawLine2D(Line2D(As.x, As.y, Bs.x, Bs.y), Color::red());
    Drawable::drawLine2D(Line2D(Bs.x, Bs.y, Cs.x, Cs.y), Color::green());
    Drawable::drawLine2D(Line2D(Cs.x, Cs.y, As.x, As.y), Color::blue());
}

void Triangle::drawWireframeColor(Uint32 c)
{
    Drawable::drawLine2D(Line2D(As.x, As.y, Bs.x, Bs.y), c);
    Drawable::drawLine2D(Line2D(Bs.x, Bs.y, Cs.x, Cs.y), c);
    Drawable::drawLine2D(Line2D(Cs.x, Cs.y, As.x, As.y), c);
}

// (v0 - P) . N
bool Triangle::isBackFaceCulling(Camera3D *cam)
{
    // Camera-triangle vector
    Vertex3D v = this->Ao - *cam->getPosition();

    return (v * this->getNormal()) >= 0;
}

Vertex3D Triangle::getCenterOfMass()
{
    Vertex3D A;

    Vertex3D At = this->Ao;
    Vertex3D Bt = this->Bo;
    Vertex3D Ct = this->Co;

    A.x = (At.x+Bt.x+Ct.x)/3;
    A.y = (At.y+Bt.y+Ct.y)/3;
    A.z = (At.z+Bt.z+Ct.z)/3;

    return A;
}

void Triangle::drawNormal(Camera3D *cam, Uint32 color)
{
    Drawable::drawVector3D( Vector3D( this->Ao, this->getNormal() ), cam, color );
}

void Triangle::scanVerticesForShadowMapping(LightPoint3D *lp)
{
    Vertex3D Aos = this->Ao;
    Vertex3D Bos = this->Bo;
    Vertex3D Cos = this->Co;

    // Pasamos por la cámara
    Vertex3D A = Ac;
    Vertex3D B = Bc;
    Vertex3D C = Cc;

    A = Transforms::NDCSpace(A, lp->cam);
    B = Transforms::NDCSpace(B, lp->cam);
    C = Transforms::NDCSpace(C, lp->cam);

    // y obtenemos los puntos en la proyección 2d
    Point2D v1 = Transforms::screenSpace(A, lp->cam);
    Point2D v2 = Transforms::screenSpace(B, lp->cam);
    Point2D v3 = Transforms::screenSpace(C, lp->cam);

    // Ordenamos los vertices y puntos por su valor en 'y'
    Maths::sortPointsByY(v1, v2, v3);
    Maths::sortVerticesByY(A, B, C);
    Maths::sortVerticesByY(Aos, Bos, Cos);

    if (v2.y == v3.y) {
        this->scanShadowMappingBottomFlatTriangle(v1, v2, v3, A, B, C, lp);
    } else if (v1.y == v2.y) {
        this->scanShadowMappingTopFlatTriangle( v1, v2, v3, A, B, C, lp);
    } else {
        // En este caso tenemos vamos a dividir los triángulos
        // para tener uno que cumpla 'bottomFlat' y el otro 'TopFlat'
        // y necesitamos un punto extra para separar ambos.
        const int x = (v1.x + ((v2.y - v1.y) / (v3.y - v1.y)) * (v3.x - v1.x));
        const int y = v2.y;

        const Point2D v4(x, y);

        // Hayamos las coordenadas baricéntricas del punto v4 respecto al triángulo v1, v2, v3
        float alpha, theta, gamma;
        Maths::getBarycentricCoordinates(alpha, theta, gamma, v4.x, v4.y, v1, v2, v3);

        const float u = alpha * A.u + theta * B.u + gamma * C.u;
        const float v = alpha * A.v + theta * B.v + gamma * C.v;

        // Creamos un nuevo vértice que representa v4 (el nuevo punto creado) en el triángulo original
        Vertex3D D = Vertex3D(
                alpha * A.x + theta * B.x + gamma * C.x,
                alpha * A.y + theta * B.y + gamma * C.y,
                alpha * A.z + theta * B.z + gamma * C.z
        );

        D.u = u; D.v = v;

        this->scanShadowMappingBottomFlatTriangle(v1, v2, v4, A, B, D, lp);
        this->scanShadowMappingTopFlatTriangle(v2, v4, v3, B, D, C, lp);
    }
}

void Triangle::scanShadowMappingTopFlatTriangle(Point2D pa, Point2D pb, Point2D pc, Vertex3D A, Vertex3D B, Vertex3D C, LightPoint3D *lp)
{
    float invslope1 = (float) (pc.x - pa.x) / (pc.y - pa.y);
    float invslope2 = (float) (pc.x - pb.x) / (pc.y - pb.y);

    float curx1 = pc.x;
    float curx2 = pc.x;

    for (int scanlineY = pc.y; scanlineY > pa.y; scanlineY--) {
        this->scanShadowMappingLine(curx1, curx2, scanlineY, pa, pb, pc, A, B, C, lp);
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

void Triangle::scanShadowMappingBottomFlatTriangle(Point2D pa, Point2D pb, Point2D pc, Vertex3D A, Vertex3D B, Vertex3D C, LightPoint3D *lp)
{
    float invslope1 = (float) (pb.x - pa.x) / (pb.y - pa.y);
    float invslope2 = (float) (pc.x - pa.x) / (pc.y - pa.y);

    float curx1 = pa.x;
    float curx2 = pa.x;

    for (int scanlineY = pa.y; scanlineY <= pb.y; scanlineY++) {
        this->scanShadowMappingLine(curx1, curx2, scanlineY, pa, pb, pc, A, B, C, lp);
        curx1 += invslope1;
        curx2 += invslope2;
    }
}


void Triangle::processPixel(int buffer_index, int x, int y, float w0, float w1, float w2, float z, float texu, float texv, float lightu, float lightv)
{
    Uint32 pixelColor;

    // Gradient
    if (EngineSetup::getInstance()->TRIANGLE_MODE_COLOR_SOLID) {
        pixelColor = (Uint32) Tools::createRGB(w0 * 255, w1 * 255, w2 * 255);
    }

    // Texture
    if (EngineSetup::getInstance()->TRIANGLE_MODE_TEXTURIZED && this->getTexture() != NULL) {
        if (getTexture()->animated && EngineSetup::getInstance()->TRIANGLE_TEXTURES_ANIMATED) {
            float cache1 = texu / EngineSetup::getInstance()->LAVA_CLOSENESS;
            float cache2 = texv / EngineSetup::getInstance()->LAVA_CLOSENESS;
            texu = (cache1 + EngineSetup::getInstance()->LAVA_INTENSITY * sin(EngineSetup::getInstance()->LAVA_SPEED * brakeza3D->timerCurrent + cache2) ) * EngineSetup::getInstance()->LAVA_SCALE;
            texv = (cache2 + EngineSetup::getInstance()->LAVA_INTENSITY * sin(EngineSetup::getInstance()->LAVA_SPEED * brakeza3D->timerCurrent + cache1) ) * EngineSetup::getInstance()->LAVA_SCALE;
        }

        pixelColor = this->processPixelTexture(texu, texv);

        Uint8 red, green, blue, alpha;
        SDL_GetRGBA(pixelColor, texture->getSurface(lod)->format, &red, &green, &blue, &alpha);

        if (alpha == 0) {
            return;
        }

        if (getLightmap()->isLightMapped() && EngineSetup::getInstance()->ENABLE_LIGHTMAPPING) {
            pixelColor = this->processPixelLightmap(pixelColor, lightu, lightv);
        }
    }

    /*if (EngineSetup::getInstance()->ENABLE_LIGHTS) {
        Vertex3D D;

        if (this->numberLightPoints > 0) {
            // Coordenadas del punto que estamos procesando en el mundo (object space)
            float x3d = w0 * Ao.x + w1 * Bo.x + w2 * Co.x;
            float y3d = w0 * Ao.y + w1 * Bo.y + w2 * Co.y;
            float z3d = w0 * Ao.z + w1 * Bo.z + w2 * Co.z;

            D = Vertex3D( x3d, y3d, z3d ); // Object space
        }

        for (int i = 0; i < this->numberLightPoints; i++) {
            if (!this->lightPoints[i]->isEnabled()) {
                continue;
            }

            // Color light apply
            float d = Maths::distanteBetweenpoints( *this->lightPoints[i]->getPosition(), D );
            pixelColor = Maths::mixColor(pixelColor, d, this->lightPoints[i], D);

            if (EngineSetup::getInstance()->ENABLE_SHADOW_CASTING) {
                Mesh3D *isMesh = dynamic_cast<Mesh3D*> (parent);

                if (isMesh != NULL && isMesh->isShadowCaster()) {
                    // Shadow test
                    Vertex3D Dl = Transforms::cameraSpace(D, this->lightPoints[i]->cam);
                    Dl = Transforms::NDCSpace(Dl, this->lightPoints[i]->cam);
                    const Point2D DP = Transforms::screenSpace(Dl, this->lightPoints[i]->cam);

                    if (Tools::isPixelInWindow(DP.x, DP.y)) {
                        float buffer_shadowmapping_z = this->lightPoints[i]->getShadowMappingBuffer(DP.x, DP.y);
                        if ( Dl.z > buffer_shadowmapping_z) {
                            pixelColor = Color::red();
                        }
                    }
                }
            }
        }
    }*/

    EngineBuffers::getInstance()->setDepthBuffer(buffer_index, z);
    EngineBuffers::getInstance()->setVideoBuffer(buffer_index, pixelColor);
}

Uint32 Triangle::processPixelTexture(float tex_u, float tex_v)
{
    float ignorablePartInt;

    // Check for inversion U
    if (!std::signbit(tex_u)) {
        tex_u = modf(tex_u , &ignorablePartInt);
    } else {
        tex_u = 1 - modf(abs(tex_u) , &ignorablePartInt);
    }

    // Check for inversion V
    if (!std::signbit(tex_v)) {
        tex_v = modf(tex_v , &ignorablePartInt);
    } else {
        tex_v = 1 - modf(abs(tex_v) , &ignorablePartInt);
    }

    if (EngineSetup::getInstance()->TEXTURES_BILINEAR_INTERPOLATION) {
        return Tools::readSurfacePixelFromBilinearUV(getTexture()->getSurface(lod), tex_u, tex_v);
    }

    return Tools::readSurfacePixelFromUV(getTexture()->getSurface(lod), tex_u, tex_v);

    /**/
}

Uint32 Triangle::processPixelLightmap(Uint32 pixelColor, float light_u, float light_v)
{
    float ignorablePartInt;

    light_u -= getLightmap()->mins[1];
    light_u /= getLightmap()->extents[1];
    light_u  = modf(abs(light_u), &ignorablePartInt);

    light_v -= getLightmap()->mins[0];
    light_v /= getLightmap()->extents[0];
    light_v  = modf(abs(light_v), &ignorablePartInt);

    Uint32 lightmap_color    = Tools::readSurfacePixelFromUV(getLightmap()->lightmap, light_v, light_u);
    Uint8 lightmap_intensity = Tools::getRedValueFromColor(lightmap_color); // RGB son iguales en un gris

    Uint8 pred, pgreen, pblue, palpha;
    SDL_GetRGBA(pixelColor, texture->getSurface(lod)->format, &pred, &pgreen, &pblue, &palpha);

    float t = lightmap_intensity * EngineSetup::getInstance()->LIGHTMAPPING_INTENSITY;

    pixelColor = (Uint32) Tools::createRGB(
            std::min(int(pred * t), 255),
            std::min(int(pgreen * t), 255),
            std::min(int(pblue * t), 255)
    );

    if (EngineSetup::getInstance()->SHOW_LIGHTMAPPING) {
        pixelColor = lightmap_color;
    }

    return pixelColor;
}

void Triangle::scanShadowMappingLine(float start_x, float end_x, int y,
                                     Point2D pa, Point2D pb, Point2D pc,
                                     Vertex3D A, Vertex3D B, Vertex3D C, LightPoint3D *lp)
{
    float offset_self_shadow = 0.25f;

    if (start_x == end_x) return;

    // left to right
    if (start_x > end_x) {
        int tmp = (int) start_x;

        start_x = end_x;
        end_x = tmp;
    }

    float alpha, theta, gamma;

    for (int x = (int) start_x; x < end_x; x++) {
        const Point2D pointFinal(x, y);

        if (Tools::isPixelInWindow(pointFinal.x, pointFinal.y)) {
            // Hayamos las coordenadas baricéntricas del punto v4 respecto al triángulo pa, pb, pc
            Maths::getBarycentricCoordinates(alpha, theta, gamma, x, y, pa, pb, pc);

            float z = alpha * A.z + theta * B.z + gamma * C.z; // Homogeneous clipspace

            float buffer_shadowmap_z = lp->getShadowMappingBuffer(pointFinal.x, pointFinal.y);
            if (buffer_shadowmap_z != NULL) {
                if ( z < buffer_shadowmap_z ) {
                    lp->setShadowMappingBuffer(pointFinal.x, pointFinal.y, z + offset_self_shadow);
                }
            }  else {
                lp->setShadowMappingBuffer(pointFinal.x, pointFinal.y, z + offset_self_shadow);
            }
        }
    }
}

Texture *Triangle::getTexture() const
{
    return texture;
}

void Triangle::setTexture(Texture *t)
{
    texture = t;
}

Texture *Triangle::getLightmap() const
{
    return lightmap;
}

void Triangle::setLightmap(Texture *t)
{
    lightmap = t;
}

void Triangle::setLightPoints(LightPoint3D **lightPoints, int number)
{
    this->lightPoints = lightPoints;
    this->numberLightPoints = number;
}

void Triangle::setClipped(bool value)
{
    this->is_clipped = value;
}

bool Triangle::isClipped()
{
    return this->is_clipped;
}

bool Triangle::isPointInside(Vertex3D v)
{
    return Maths::PointInTriangle(v, Ao, Bo, Co);
}

int Triangle::processLOD()
{
    int lod = EngineSetup::getInstance()->LOAD_OF_DETAIL;

    if (getTexture() == NULL) return 0;

    if (getTexture()->isMipMapped() && EngineSetup::getInstance()->ENABLE_MIPMAPPING) {
        float area_screen = Maths::TriangleArea(As.x, As.y, Bs.x, Bs.y, Cs.x, Cs.y);
        float area_texture = getTexture()->getAreaForVertices(A, B, C, 1);

        float r = area_texture / area_screen;

        int triangle_lod = (int) floor(r);
        int clamped_lod = 1;

        // Range LOD selection
        if (triangle_lod < 10) {
            clamped_lod = 1;
        } else if (triangle_lod >= 10 && triangle_lod  < 15) {
            clamped_lod = 2;
        } else if (triangle_lod >= 15 && triangle_lod  < 25) {
            clamped_lod = 4;
        } else if (triangle_lod > 25) {
            clamped_lod = 8;
        }

        lod =  clamped_lod;
    }

    return lod;
}

bool Triangle::testForClipping(Camera3D *cam)
{
    // clip against planes
    Vector3D edges[3];
    edges[0] = Vector3D(Ao, Bo);
    edges[1] = Vector3D(Bo, Co);
    edges[2] = Vector3D(Co, Ao);

    for (int i = EngineSetup::getInstance()->LEFT_PLANE ; i <= EngineSetup::getInstance()->BOTTOM_PLANE ; i++) {
        for (int e = 0 ; e < 3 ; e++) {
            if ( Maths::isVector3DClippingPlane(cam->frustum->planes[i], edges[e]) > 1 ) {
                return true;
            }
        }
    }

    return false;
}

int Triangle::getId() const {
    return id;
}

void Triangle::setId(int id) {
    Triangle::id = id;
}

Plane Triangle::plane()
{
    return Plane(this->Ao, this->Bo, this->Co);
}


OCLTriangle Triangle::getOpenCL()
{
    OCLTriangle ot;

    ot.id = 69;
    ot.As_x = As.x;
    ot.As_y = As.y;
    ot.Bs_x = Bs.x;
    ot.Bs_y = Bs.y;
    ot.Cs_x = Cs.x;
    ot.Cs_y = Cs.y;

    ot.An_z = An.z;
    ot.Bn_z = Bn.z;
    ot.Cn_z = Cn.z;

    return ot;
}