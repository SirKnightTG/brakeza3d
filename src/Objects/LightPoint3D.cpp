
#include "../../headers/Objects/LightPoint3D.h"
#include "../../headers/Render/Billboard.h"
#include "../../headers/Render/EngineBuffers.h"

LightPoint3D::LightPoint3D()
{
    this->billboard = new Billboard();
    this->billboard->loadTexture( EngineSetup::getInstance()->ICON_LIGHTPOINTS_DEFAULT );

    cam = new Camera3D();
    cam->setLabel("Camera LightPoint");
    cam->setPosition(this->position);
    cam->setRotation(this->rotation);

    this->sizeBuffer = EngineSetup::getInstance()->SCREEN_WIDTH * EngineSetup::getInstance()->SCREEN_HEIGHT;
    shadowMappingBuffer = new float[sizeBuffer];
}

void LightPoint3D::syncFrustum()
{
    this->cam->setPosition(this->position);
    this->cam->setRotation(this->rotation);

    this->cam->frustum->position  = this->cam->position;
    this->cam->frustum->direction = this->cam->eyeVector();
    this->cam->frustum->up        = this->cam->upVector();
    this->cam->frustum->right     = this->cam->rightVector();

    this->cam->frustum->updateCenters();
    this->cam->frustum->updatePoints();
    this->cam->frustum->updatePlanes();
}

void LightPoint3D::clearShadowMappingBuffer()
{
    std::fill(shadowMappingBuffer, shadowMappingBuffer + sizeBuffer, NULL);
}

float LightPoint3D::getShadowMappingBuffer(int x, int y)
{
    return shadowMappingBuffer[(y * EngineSetup::getInstance()->SCREEN_WIDTH ) + x ];
}

float LightPoint3D::setShadowMappingBuffer(int x, int y, float value)
{
    shadowMappingBuffer[(y * EngineSetup::getInstance()->SCREEN_WIDTH ) + x ] = value;
}

void LightPoint3D::setColor(int r, int g, int b)
{
    Uint32 color = ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);

    LightPoint3D::color = color;

    ImVec4 c = ImColor(r, g, b, 255);

    this->imgui_color = c;
}