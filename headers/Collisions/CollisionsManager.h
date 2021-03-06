
#ifndef BRAKEDA3D_COLLISIONSMANAGER_H
#define BRAKEDA3D_COLLISIONSMANAGER_H


#include "../Physics/Mesh3DGhost.h"
#include "../Render/BSPMap.h"
#include "../Render/PhysicsDebugDraw.h"
#include "../Physics/Mesh3DBody.h"
#include "../Physics/SpriteDirectional3DBody.h"
#include "../2D/WeaponsManager.h"
#include "CollisionResolver.h"

class CollisionsManager {
private:
    std::vector<CollisionResolver*> collisions;
public:
    Camera3D *camera;
    Mesh3DGhost *triggerCamera;
    BSPMap *bspMap;
    std::vector<Object3D*> *gameObjects;
    WeaponsManager *weaponManager;
    std::vector<Triangle *> *visibleTriangles;

    ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    btDefaultCollisionConfiguration* collisionConfiguration;
    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    btCollisionDispatcher* dispatcher;
    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    btBroadphaseInterface* overlappingPairCache;
    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
    PhysicsDebugDraw* debugDraw;

    CollisionsManager();

    void initBulletSystem();
    void makeGhostForCamera();
    void checkCollisionsForTriggerCamera();
    void checkCollisionsForAll();

    btDiscreteDynamicsWorld *getDynamicsWorld() const;
    void setDynamicsWorld(btDiscreteDynamicsWorld *dynamicsWorld);

    Camera3D *getCamera() const;
    void setCamera(Camera3D *camera);

    Mesh3DGhost *getTriggerCamera() const;
    void setTriggerCamera(Mesh3DGhost *triggerCamera);

    BSPMap *getBspMap() const;
    void setBspMap(BSPMap *bspMap);

    std::vector<Object3D *> *getGameObjects() const;
    void setGameObjects(std::vector<Object3D *> *gameObjects);

    std::vector<Triangle *> &getVisibleTriangles();
    void setVisibleTriangles(std::vector<Triangle *> &visibleTriangles);

    WeaponsManager *getWeaponManager() const;

    void setWeaponManager(WeaponsManager *weaponManager);

    bool needsCollision(const btCollisionObject* body0, const btCollisionObject* body1);

    void updatePhysicObjects();

    Vertex3D stepSimulation();

    std::vector<CollisionResolver *> &getCollisions();

    void setCollisions(const std::vector<CollisionResolver *> &collisions);

};


#endif //BRAKEDA3D_COLLISIONSMANAGER_H
