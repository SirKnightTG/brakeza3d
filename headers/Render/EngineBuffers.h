#ifndef SDL2_3D_ENGINE_ZBUFFER_H
#define SDL2_3D_ENGINE_ZBUFFER_H

#include <iostream>
#include <SDL_surface.h>
#include <SDL_audio.h>
#include <SDL_mixer.h>
#include "Timer.h"
#include "../Objects/Sprite3D.h"
#include "../Game/NPCEnemyBody.h"
#include "../Misc/SoundPackage.h"

// Singleton
class EngineBuffers {

private:
    static EngineBuffers* instance;

    EngineBuffers();
    ~EngineBuffers();

public:
    static EngineBuffers* getInstance();

    float  *depthBuffer;
    Uint32 *videoBuffer;
    int    sizeBuffers;

    int *firePixelsBuffer;

    OCLTriangle *OCLTrianglesBuffer;
    int numOCLTriangles = 0;

    // sounds
    SoundPackage *soundPackage;

    // sprite templates
    Sprite3D                   *goreDecalTemplates;
    Sprite3D                   *bloodTemplates;
    Sprite3D                   *gibsTemplate;
    std::vector<NPCEnemyBody*> enemyTemplates;

    // Fire colors for menu fire effect
    Uint32 fireColors[37];
    int rgbs[111] = {
        0x07, 0x07, 0x07,
        0x1F, 0x07, 0x07,
        0x2F, 0x0F, 0x07,
        0x47, 0x0F, 0x07,
        0x57, 0x17, 0x07,
        0x67, 0x1F, 0x07,
        0x77, 0x1F, 0x07,
        0x8F, 0x27, 0x07,
        0x9F, 0x2F, 0x07,
        0xAF, 0x3F, 0x07,
        0xBF, 0x47, 0x07,
        0xC7, 0x47, 0x07,
        0xDF, 0x4F, 0x07,
        0xDF, 0x57, 0x07,
        0xDF, 0x57, 0x07,
        0xD7, 0x5F, 0x07,
        0xD7, 0x5F, 0x07,
        0xD7, 0x67, 0x0F,
        0xCF, 0x6F, 0x0F,
        0xCF, 0x77, 0x0F,
        0xCF, 0x7F, 0x0F,
        0xCF, 0x87, 0x17,
        0xC7, 0x87, 0x17,
        0xC7, 0x8F, 0x17,
        0xC7, 0x97, 0x1F,
        0xBF, 0x9F, 0x1F,
        0xBF, 0x9F, 0x1F,
        0xBF, 0xA7, 0x27,
        0xBF, 0xA7, 0x27,
        0xBF, 0xAF, 0x2F,
        0xB7, 0xAF, 0x2F,
        0xB7, 0xB7, 0x2F,
        0xB7, 0xB7, 0x37,
        0xCF, 0xCF, 0x6F,
        0xDF, 0xDF, 0x9F,
        0xEF, 0xEF, 0xC7,
        0xFF, 0xFF, 0xFF
    };

    NPCEnemyBody* getEnemyTemplateForClassname(std::string classname);

    void  clearDepthBuffer();
    float getDepthBuffer(int x, int y);
    float getDepthBuffer(int i);

    void setDepthBuffer(const int x, const int y, const float value);
    void setDepthBuffer(const int i, const float value);

    float getVideoBuffer(int x, int y);
    void  clearVideoBuffer();
    void  setVideoBuffer(const int x, const int y, Uint32 value);
    void  setVideoBuffer(const int i, Uint32 value);
    void  flipVideoBufferToSurface(SDL_Surface *);

    void setNumOCLTriangles(int num);
    void addOCLTriangle(OCLTriangle);

    void makeFireColors();
    void fireShaderSetup();

    void loadSounds();
};

#endif
