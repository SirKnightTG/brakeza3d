#ifndef SDL2_3D_ENGINE_ZBUFFER_H
#define SDL2_3D_ENGINE_ZBUFFER_H

#include <iostream>
#include <SDL_surface.h>
#include "Timer.h"

// Singleton
class EngineBuffers
{

private:
    static EngineBuffers* instance;

    EngineBuffers();
    ~EngineBuffers();

public:

    float *depthBuffer;
    Uint32 *videoBuffer;

    int sizeBuffers;
    static EngineBuffers* getInstance();

    int pixelesDrawed = 0;
    int pixelesOutOfWindow = 0;
    int pixelesBehindOfCamera = 0;

    int trianglesDrawed = 0;
    int trianglesHidenByFaceCuling = 0;
    int trianglesNoTexture = 0;
    int trianglesOutFrustum = 0;
    int trianglesClippingCreated = 0;

    SDL_Surface  *screenSurface;

    void clearDepthBuffer();
    float getDepthBuffer(int x, int y);
    float setDepthBuffer(int x, int y, float value);

    void clearVideoBuffer();
    float getVideoBuffer(int x, int y);
    void setVideoBuffer(int x, int y, Uint32 value);
    void flipVideoBuffer(SDL_Surface *);

    void resetBenchmarkValues();
    void consoleInfo();

    SDL_Surface *getScreenSurface() const;

    void setScreenSurface(SDL_Surface *screenSurface);
};

#endif
