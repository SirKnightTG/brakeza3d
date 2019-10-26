#include "Game.h"
#include "headers/Render/Maths.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "headers/Objects/Decal.h"
#include "headers/Physics/Sprite3DBody.h"
#include "headers/Brakeza3D.h"
#include "headers/Render/EngineBuffers.h"

Game::Game()
{
}

void Game::run()
{
    this->onStart();
    this->mainLoop();
    this->onEnd();

    Close();
}

void Game::onStart()
{
    Engine::onStart();

    //this->loadDemoObjects();
}

void Game::mainLoop()
{
    ImGuiIO& io = ImGui::GetIO();
    while(!finish) {

        this->preUpdate();

        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL2_ProcessEvent(&e);
            if (EngineSetup::getInstance()->CAMERA_MOUSE_ROTATION) {
                Brakeza3D::get()->getController()->handleMouse(&this->e, Brakeza3D::get()->getCamera(),
                                                               Brakeza3D::get()->getCollisionManager()->getDynamicsWorld(),
                                                               Brakeza3D::get()->getSceneObjects(),
                                                               Brakeza3D::get()->getTimer(),
                                                               Brakeza3D::get()->getMenuManager(),
                                                               Brakeza3D::get()->getWeaponsManager());
            }
            Brakeza3D::get()->getController()->handleKeyboard(&this->e, Brakeza3D::get()->getCamera(), this->finish,
                                                              Brakeza3D::get()->getCollisionManager()->getDynamicsWorld(),
                                                              Brakeza3D::get()->getSceneObjects(),
                                                              Brakeza3D::get()->getTimer(),
                                                              Brakeza3D::get()->getMenuManager(),
                                                              Brakeza3D::get()->getWeaponsManager());
        }

        if (finish) continue;

        // Check array Uint8 *keyboard
        Brakeza3D::get()->getController()->handleKeyboardContinuous(&this->e, Brakeza3D::get()->getCamera(), this->finish,
                                                                    Brakeza3D::get()->getCollisionManager()->getDynamicsWorld(),
                                                                    Brakeza3D::get()->getSceneObjects(),
                                                                    Brakeza3D::get()->getTimer(),
                                                                    Brakeza3D::get()->getMenuManager(),
                                                                    Brakeza3D::get()->getWeaponsManager());

        // Checks pre update frame
        this->postUpdate();

        // game level update
        this->onUpdate();

        // Update window
        Engine::onUpdateWindow();
    }
}

void Game::onUpdate()
{
    // Core onUpdate
    Engine::onUpdate();

    onUpdateIA();

    // Lighting example
    /*Vertex3D startPoint = Vertex3D(45, -2, 40);
    Vertex3D endPoint   = Vertex3D(70, -2, 40);
    Drawable::drawLightning(camera, startPoint, endPoint);*/

    //this->bspMap->recastWrapper->drawNavMeshPoints();

    // Calc Pathfinding example
    //Vertex3D A(*this->getObjectByLabel("BSPEntity_21 (monster)")->getPosition());
    //Vertex3D B(*this->getObjectByLabel("BSPEntity_245 (monster)")->getPosition());
    //Vector3D t(A, B);
    //std::vector<Vertex3D> points;
    //this->bspMap->recastWrapper->getPathBetween(A, B, points);
    //this->bspMap->recastWrapper->drawPathSegments(points);
    //Drawable::drawVector3D(t, camera, Color::magenta());

    // Raycasting example
    /*Vertex3D A(-13, 16, 191);
    Vertex3D B(*camera->getPosition());
    bool hitResult = this->bspMap->recastWrapper->rayCasting(A, B);*/
}

void Game::preUpdate()
{
    // Core preUpdate
    Engine::preUpdate();
}

void Game::onEnd()
{
    Engine::onEnd();
}

void Game::loadDemoObjects()
{
    LightPoint3D *lp1 = new LightPoint3D();
    lp1->setEnabled(false);
    lp1->setLabel("LightPoint1");
    lp1->setPosition(Vertex3D(1, 1.5f, -1));
    lp1->setColor( 255, 0, 0 );
    Brakeza3D::get()->addLightPoint(lp1, "l1");

    LightPoint3D *lp2 = new LightPoint3D();
    lp2->setEnabled(false);
    lp2->setLabel("LightPoint2");
    lp2->setPosition(Vertex3D(-0.4, 1, -1));
    lp2->setColor( 0, 255, 0 );
    Brakeza3D::get()->addLightPoint(lp2, "l2");

    LightPoint3D *lp3 = new LightPoint3D();
    lp3->setEnabled(false);
    lp3->setLabel("LightPoint3");
    lp3->setPosition(Vertex3D(2, 1, -1));
    lp3->setColor( 0, 0, 255 );
    Brakeza3D::get()->addLightPoint(lp3, "l3");

    // mono
    Mesh3D *monkey = new Mesh3D();
    monkey->setEnabled(false);
    monkey->setPosition(Vertex3D(1, 1, 28));
    //monkey->setLightPoints(Engine::lightPoints);
    monkey->loadOBJBlender("../assets/models/mono.obj");
    monkey->setShadowCaster(true);
    Brakeza3D::get()->addObject3D(monkey, "monkey");

    // hammer
    Mesh3D *hammer = new Mesh3D();
    hammer->setEnabled(false);
    hammer->setPosition(Vertex3D(5, 7.5, 78.2));
    hammer->loadOBJBlender("../assets/models/hammer.obj");
    hammer->setShadowCaster(true);
    Brakeza3D::get()->addObject3D(hammer, "hammer");

    // ball
    Mesh3D *wolf = new Mesh3D();
    wolf->setEnabled(false);
    wolf->loadOBJBlender("../assets/models/Wolf.obj");
    wolf->setShadowCaster(true);
    Brakeza3D::get()->addObject3D(wolf, "wolf");

    // cubo
    Mesh3D *cube = new Mesh3D();
    cube->setEnabled(false);
    cube->setPosition(*Brakeza3D::get()->getCamera()->getPosition());
    cube->loadOBJBlender("../assets/models/cubo.obj");
    Brakeza3D::get()->addObject3D(cube, "cube");

    // decal
    Decal *decal = new Decal();
    decal->setPosition(Vertex3D(2, -12, 76.5));
    decal->setPosition(Vertex3D(52, -12, 75.5));
    decal->setupCube(10, 10, 10);
    decal->setRotation(M3::getMatrixRotationForEulerAngles(0, 0, 0));
    decal->getSprite()->linkTextureAnimation(EngineBuffers::getInstance()->goreTemplate);
    decal->getSprite()->setAnimation(Tools::random(0, 10));
    decal->cube->setPosition(*decal->getPosition());
    decal->cube->update();
    Brakeza3D::get()->addObject3D(decal, "decal");

    // triangle
    Mesh3D *triangle = new Mesh3D();
    triangle->setScale(0.01);
    triangle->setEnabled(true);
    triangle->setPosition(Vertex3D(1, 1, 30));
    triangle->setRotation( M3(1, 0, 3) );
    triangle->loadOBJBlender("../assets/models/triangle_2uv.obj");
    Brakeza3D::get()->addObject3D(triangle, "triangle");

    // plane
    Mesh3D *plane = new Mesh3D();
    plane->setEnabled(false);
    plane->setPosition(Vertex3D(544, -32, 613));
    plane->setRotation( M3(-90, -45, 0) );
    plane->loadOBJBlender("../assets/models/plane.obj");
    Brakeza3D::get()->addObject3D(plane, "plane");

    // marine (sprite directional)
    SpriteDirectional3D *marine = new SpriteDirectional3D();
    marine->setEnabled(true);
    marine->setPosition(Vertex3D(2, 0, 10));
    marine->setRotation( M3(0, -90, 0) );
    marine->setTimer(Brakeza3D::get()->getTimer());
    marine->addAnimationDirectional2D("soldier/walk", 4, 20,  false, -1);
    marine->addAnimationDirectional2D("soldier/fire", 2, 20, false, -1);
    marine->addAnimationDirectional2D("soldier/injuried", 1, 20, false, -1);
    marine->addAnimationDirectional2D("soldier/dead", 5, 20, true, 1);
    marine->addAnimationDirectional2D("soldier/explosion", 8, 20, true, 1);
    marine->setAnimation(EngineSetup::getInstance()->SpriteSoldierAnimations::SOLDIER_WALK);
    Brakeza3D::get()->addObject3D(marine, "marine");

    // skull (sprite directional)
    SpriteDirectional3D *skull = new SpriteDirectional3D();
    skull->setEnabled(false);
    skull->setPosition(Vertex3D(5, 0, -10));
    skull->setTimer(Brakeza3D::get()->getTimer());
    skull->addAnimationDirectional2D("skull/idle", 5, 20, false, -1);
    skull->setAnimation(EngineSetup::getInstance()->SpriteSoldierAnimations::SOLDIER_WALK);
    Brakeza3D::get()->addObject3D(skull, "skull");

    // caco (sprite directional)
    SpriteDirectional3D *caco = new SpriteDirectional3D();
    caco->setEnabled(false);
    caco->setPosition(Vertex3D(20, 0, -10));
    caco->setTimer(Brakeza3D::get()->getTimer());
    caco->addAnimationDirectional2D("cacodemon/walk", 6, 20, false, -1);
    caco->addAnimationDirectional2D("cacodemon/dead", 6, 20, false, -1);
    caco->setAnimation(EngineSetup::getInstance()->SpriteDoom2CacodemonAnimations::FLY);
    Brakeza3D::get()->addObject3D(caco, "caco");

    // marine ( sprite )
    Sprite3D *doomFace = new Sprite3D();
    doomFace->setEnabled(false);
    doomFace->setPosition( Vertex3D(2, 1, 15) );
    doomFace->setTimer(Brakeza3D::get()->getTimer());
    doomFace->addAnimation("doom_face/face", 3, 10);
    doomFace->setAnimation(EngineSetup::getInstance()->SpriteGuyAnimations::NORMAL);
    doomFace->getBillboard()->setDimensions(1, 1);
    Brakeza3D::get()->addObject3D(doomFace, "doomFace");

    // marineBody ( sprite )
    Sprite3DBody *doomFaceBody = new Sprite3DBody();
    doomFaceBody->setLabel("doomFaceBody");
    doomFaceBody->setEnabled(false);
    doomFaceBody->setPosition( Vertex3D(2, 1, 15) );
    doomFaceBody->setTimer(Brakeza3D::get()->getTimer());
    doomFaceBody->addAnimation("doom_face/face", 3, 10);
    doomFaceBody->setAnimation(EngineSetup::getInstance()->SpriteGuyAnimations::NORMAL);
    doomFaceBody->getBillboard()->setDimensions(1, 1);
    doomFaceBody->makeRigidBody(1.0f, Brakeza3D::get()->getSceneObjects(), Brakeza3D::get()->getCamera(),
                                Brakeza3D::get()->getCollisionManager()->getDynamicsWorld(), doomFaceBody);
    //this->addObject3D(doomFaceBody, "doomFaceBody");

    // cubo physics
    Mesh3DBody *cuboPhysic = new Mesh3DBody();
    cuboPhysic->setLabel("cuboPhysics");
    cuboPhysic->setEnabled(false);
    cuboPhysic->setPosition(Vertex3D(54, -16, 87));
    cuboPhysic->loadOBJBlender("../assets/models/cubo.obj");
    cuboPhysic->makeRigidBody(1.0f, Brakeza3D::get()->getSceneObjects(), Brakeza3D::get()->getCamera(),
                              Brakeza3D::get()->getCollisionManager()->getDynamicsWorld(), false);
    //this->addObject3D(cuboPhysic, "cuboPhysic");

    /*Mesh3DGhost *cuboPhysicGhost = new Mesh3DGhost();
    cuboPhysicGhost->setEnabled(true);
    cuboPhysicGhost->setLightPoints(Engine::lightPoints, Engine::numberLightPoints);
    cuboPhysicGhost->setPosition(Vertex3D(52, -0.2, 87));
    cuboPhysicGhost->loadOBJBlender("../assets/models/cubo.obj");
    cuboPhysicGhost->makeGhostBody(Engine::camera, this->dynamicsWorld, true);
    this->addObject3D(cuboPhysicGhost, "cuboPhysicGhost");*/

    // cube3d
    Cube3D *oCube = new Cube3D(1,  7, 5);
    oCube->setEnabled(false);
    oCube->setPosition(Vertex3D(1, 1, 1));
    Brakeza3D::get()->addObject3D(oCube, "oCube");
}

void Game::onUpdateIA()
{
    std::vector<Object3D *>::iterator itObject3D;
    for ( itObject3D = Brakeza3D::get()->getSceneObjects().begin(); itObject3D !=
                                                                    Brakeza3D::get()->getSceneObjects().end(); itObject3D++) {
        Object3D *object = *(itObject3D);

        if (!Brakeza3D::get()->getCamera()->frustum->isPointInFrustum(*object->getPosition())) {
            continue;
        }

        NPCEnemyBody *enemy = dynamic_cast<NPCEnemyBody*> (object);
        if (enemy != NULL) {
            if (enemy->isDead()) continue;

            Vertex3D A = *object->getPosition();
            Vertex3D B = *Brakeza3D::get()->getCamera()->getPosition();

            enemy->points.clear();
            Brakeza3D::get()->getBSP()->recastWrapper->getPathBetween(A, B, enemy->points );
            enemy->evalStatusMachine(
                    Brakeza3D::get()->getBSP()->recastWrapper->rayCasting(A, B),
                    Vector3D(A, B).getComponent().getModule(),
                    Brakeza3D::get()->getCamera(),
                    Brakeza3D::get()->getCollisionManager()->getDynamicsWorld(),
                    Brakeza3D::get()->getSceneObjects()
            );
        }
    }
}