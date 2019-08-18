#include "CollisionResolver.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "../Render/Engine.h"

CollisionResolver::CollisionResolver()
{

}

void CollisionResolver::initBulletSystem()
{
    ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    this->collisionConfiguration = new btDefaultCollisionConfiguration();

    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    this->dispatcher = new btCollisionDispatcher(collisionConfiguration);

    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    this->overlappingPairCache = new btDbvtBroadphase();

    btVector3 worldMin(-500,-500,-500);
    btVector3 worldMax(500,500,500);
    btAxisSweep3* sweepBP = new btAxisSweep3(worldMin,worldMax);
    this->overlappingPairCache = sweepBP;

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    this->solver = new btSequentialImpulseConstraintSolver;

    /// Debug drawer
    this->debugDraw = new PhysicsDebugDraw(this->camera);

    this->dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
    this->dynamicsWorld->setGravity(btVector3(0, EngineSetup::getInstance()->gravity.y, 0));

    this->overlappingPairCache->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

    this->camera->kinematicController->setGravity( dynamicsWorld->getGravity() );
    this->camera->kinematicController->setFallSpeed(00);

    this->dynamicsWorld->addCollisionObject(this->camera->m_ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter);
    this->dynamicsWorld->addAction(this->camera->kinematicController);

    this->dynamicsWorld->setDebugDrawer(debugDraw);
    this->dynamicsWorld->getDebugDrawer()->setDebugMode(PhysicsDebugDraw::DBG_DrawWireframe);

    this->makeGhostForCamera();

}

void CollisionResolver::makeGhostForCamera()
{
    triggerCamera = new Mesh3DGhost();
    triggerCamera->setLabel("triggerCamera");
    triggerCamera->setEnabled(true);
    triggerCamera->setPosition(*camera->getPosition());
    triggerCamera->getGhostObject()->setCollisionShape(camera->kinematicController->getGhostObject()->getCollisionShape());
    triggerCamera->getGhostObject()->setUserPointer(triggerCamera);
    dynamicsWorld->addCollisionObject(triggerCamera->getGhostObject(), collisionGroups::CameraTrigger, collisionGroups::DefaultFilter|collisionGroups::BSPHullTrigger);
}

void CollisionResolver::checkTriggerCamera()
{
    for (int i = 0; i < this->triggerCamera->getGhostObject()->getNumOverlappingObjects(); i++) {
        const btCollisionObject *obj = this->triggerCamera->getGhostObject()->getOverlappingObject(i);
        Mesh3D *brkObjectB = (Mesh3D *) obj->getUserPointer();

        // No siempre tienen por qué ser Mesh
        if (brkObjectB != NULL) {
            if (brkObjectB->getLabel().find("hull") != std::string::npos) {
                int entityIndex = brkObjectB->getBspEntityIndex();
                char *classname = bspMap->getEntityValue(entityIndex, "classname");

                if (!strcmp(classname, "trigger_teleport")) {
                    int targetEntityId = bspMap->getIndexOfFirstEntityByTargetname(bspMap->getEntityValue(entityIndex, "target") );
                    if (targetEntityId >= 0) {

                        if (this->bspMap->hasEntityAttribute(targetEntityId, "origin")) {
                            char *value = bspMap->getEntityValue(targetEntityId, "origin");
                            char *angle = bspMap->getEntityValue(targetEntityId, "angle");
                            Vertex3D teleportPos = bspMap->parsePositionFromEntityAttribute(value);

                            float BSP_YOffset = -5;
                            float BSP_ZOffset = 0;

                            int angleInt = atoi( std::string(angle).c_str() );
                            camera->yaw   = 90-angleInt;
                            camera->pitch = 0;
                            camera->roll  = 0;

                            btVector3 btPos = btVector3(teleportPos.x, teleportPos.y+BSP_YOffset, teleportPos.z+BSP_ZOffset);

                            btTransform initialTransform;
                            initialTransform.setOrigin( btPos );

                            this->camera->kinematicController->getGhostObject()->setWorldTransform(initialTransform);

                            if (EngineSetup::getInstance()->LOG_COLLISION_OBJECTS) {
                                Logging::getInstance()->getInstance()->Log( "[LOG_COLLISION_OBJECTS] teleporting to " +std::string(bspMap->getEntityValue(entityIndex, "target")));
                            }
                        }
                    }
                }

                if (!strcmp(classname, "func_door")) {
                    //Tools::writeTextCenter(Engine::renderer, Engine::font, Color::white(), std::string("func_door") );
                }

                if (!strcmp(classname, "trigger_multiple")) {
                    // check for message response
                    if (strlen(bspMap->getEntityValue(entityIndex, "message")) > 0) {
                        //Tools::writeTextCenter(Engine::renderer, Engine::font, Color::white(), std::string(bspMap->getEntityValue(entityIndex, "message")) );
                    }
                }
            }
        }

        if (EngineSetup::getInstance()->LOG_COLLISION_OBJECTS) {
            Logging::getInstance()->getInstance()->Log("[LOG_COLLISION_OBJECTS] Collision between triggerCamera and " + brkObjectB->getLabel());
        }
    }
}

void CollisionResolver::checkAll()
{
// All collisions pairs
    int numManifolds = this->dynamicsWorld->getDispatcher()->getNumManifolds();
    if (EngineSetup::getInstance()->BULLET_CHECK_ALL_PAIRS) {
        for (int i = 0; i < numManifolds; i++) {
            btPersistentManifold *contactManifold = this->dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
            if (contactManifold->getNumContacts() > 0) {
                const btCollisionObject *obA = contactManifold->getBody0();
                const btCollisionObject *obB = contactManifold->getBody1();

                Object3D *brkObjectA = (Object3D *) obA->getUserPointer();
                Object3D *brkObjectB = (Object3D *) obB->getUserPointer();

                BSPMap *oMap = dynamic_cast<BSPMap*> (brkObjectB);
                if (oMap != NULL) {

                    if (brkObjectA->getLabel() == "projectile") {
                        dynamicsWorld->removeCollisionObject((btCollisionObject *) obA);
                        //this->removeObject3D(brkObjectA);
                    }

                    if (EngineSetup::getInstance()->LOG_COLLISION_OBJECTS) {
                        Logging::getInstance()->getInstance()->Log("[AllPairs] Collision between " + brkObjectA->getLabel() + " and BSPMap");
                    }
                }

                SpriteDirectional3D *oSpriteDirectional = dynamic_cast<SpriteDirectional3D*> (brkObjectB);
                if (oSpriteDirectional != NULL) {

                    Enemy *oSpriteDirectionalEnemyA = dynamic_cast<Enemy*> (brkObjectA);
                    Enemy *oSpriteDirectionalEnemyB = dynamic_cast<Enemy*> (brkObjectB);

                    if ( ( oSpriteDirectionalEnemyA != NULL || oSpriteDirectionalEnemyB != NULL) &&
                         ( brkObjectA->getLabel() == "projectile" || brkObjectB->getLabel() == "projectile" )
                            ) {
                        if (EngineSetup::getInstance()->LOG_COLLISION_OBJECTS) {
                            Logging::getInstance()->getInstance()->Log("[AllPairs] Collision between projectile and Enemy");
                        }
                        if (oSpriteDirectionalEnemyA != NULL) {
                            oSpriteDirectionalEnemyA->stamina -= 10; //weapon->getCurrentWeaponType()->getDamage();

                            if (oSpriteDirectionalEnemyA->stamina < 0) {
                                //SpriteDirectional3D *tmp = dynamic_cast<SpriteDirectional3D*> (brkObjectA);
                                //tmp->setAnimation(EngineSetup::getInstance()->SpriteDoom2SoldierAnimations::SOLDIER_DEAD);
                                //dynamicsWorld->removeCollisionObject( (btCollisionObject *) obA );
                            }

                        } else {
                            dynamicsWorld->removeCollisionObject( (btCollisionObject *) obB );
                            //this->removeObject3D( brkObjectB );
                        }

                        if (oSpriteDirectionalEnemyB != NULL) {
                            oSpriteDirectionalEnemyB->stamina--;
                            if (oSpriteDirectionalEnemyB->stamina < 0) {
                                //SpriteDirectional3D *tmp = dynamic_cast<SpriteDirectional3D*> (brkObjectB);
                                //tmp->setAnimation(EngineSetup::getInstance()->SpriteDoom2SoldierAnimations::SOLDIER_DEAD);
                                //dynamicsWorld->removeCollisionObject( (btCollisionObject *) obB );

                            }

                        } else {
                            dynamicsWorld->removeCollisionObject( (btCollisionObject *) obA );
                            //this->removeObject3D( brkObjectA );
                        }
                    }

                }

                Mesh3D *oMesh = dynamic_cast<Mesh3D*> (brkObjectB);
                if (oMesh != NULL) {
                    int originalEntityIndex = oMesh->getBspEntityIndex();

                    if (originalEntityIndex > 0) {
                        char *classname = bspMap->getEntityValue(originalEntityIndex, "classname");
                        char *currentTargetName = bspMap->getEntityValue(originalEntityIndex, "targetname");

                        if ( !strcmp(classname, "func_door") ) {
                            if (EngineSetup::getInstance()->LOG_COLLISION_OBJECTS) {
                                Logging::getInstance()->getInstance()->Log("[AllPairs] Collision with func_door");
                            }
                            if (!bspMap->hasEntityAttribute(originalEntityIndex, "targetname")) {
                                // No tiene targetname
                                Mesh3DBody *originalBody = dynamic_cast<Mesh3DBody*> (brkObjectB);
                                if (originalBody != NULL) {
                                    this->moveMesh3DBody(originalBody, originalEntityIndex);
                                    if (EngineSetup::getInstance()->LOG_COLLISION_OBJECTS) {
                                        Logging::getInstance()->getInstance()->Log("moveMesh3DBody: " + originalBody->getLabel());
                                    }
                                }
                            } else {
                                int targetRemoteEntityId = bspMap->getIndexOfFirstEntityByTarget( currentTargetName );
                                char *classnameRemote = bspMap->getEntityValue(targetRemoteEntityId, "classname");

                                if ( !strcmp(classnameRemote, "trigger_counter") ) {
                                    for (int k = 0; k < this->gameObjects->size(); k++) {
                                        Mesh3D *oRemoteMesh = dynamic_cast<Mesh3D*> ((*this->gameObjects)[k]);
                                        if (oRemoteMesh != NULL) {
                                            if (oRemoteMesh->getBspEntityIndex() == targetRemoteEntityId) {
                                                Mesh3DGhost *oRemoteGhost = dynamic_cast<Mesh3DGhost*> (oRemoteMesh);
                                                int currentCounter = oRemoteGhost->currentTriggerCounter;

                                                char *countValue = bspMap->getEntityValue(targetRemoteEntityId, "count");
                                                int countValueInt = atoi( std::string(countValue).c_str() );

                                                if (countValueInt == currentCounter) {
                                                    Mesh3DBody *originalBody = dynamic_cast<Mesh3DBody*> (brkObjectB);

                                                    this->moveMesh3DBody(originalBody, originalEntityIndex);
                                                } else {
                                                    if (strlen(bspMap->getEntityValue(originalEntityIndex, "message")) > 0) {
                                                        //Tools::writeTextCenter(Engine::renderer, Engine::font, Color::white(), std::string(bspMap->getEntityValue(originalEntityIndex, "message")) );
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        //***************************
                        if ( !strcmp(classname, "func_button") ) {
                            char *targetRemote = bspMap->getEntityValue(originalEntityIndex, "target");
                            int targetRemoteEntityId = bspMap->getIndexOfFirstEntityByTargetname(targetRemote );

                            Mesh3DBody *originalBody = dynamic_cast<Mesh3DBody*> (brkObjectB);
                            this->moveMesh3DBody(originalBody, originalEntityIndex);

                            if (targetRemoteEntityId >= 0) {
                                char *classnameRemote = bspMap->getEntityValue(targetRemoteEntityId, "classname");
                                if (!strcmp(classnameRemote, "func_door")) {
                                    // Buscamos algún objeto cuya BSPEntity coincida
                                    for (int k = 0; k < this->gameObjects->size(); k++) {
                                        Mesh3D *oRemoteMesh = dynamic_cast<Mesh3D*> ((*this->gameObjects)[k]);
                                        if (oRemoteMesh != NULL) {
                                            if (oRemoteMesh->getBspEntityIndex() == targetRemoteEntityId) {

                                                Mesh3DBody *oRemoteBody = dynamic_cast<Mesh3DBody*> (oRemoteMesh);
                                                this->moveMesh3DBody(oRemoteBody, targetRemoteEntityId);

                                                if (EngineSetup::getInstance()->LOG_COLLISION_OBJECTS) {
                                                    Logging::getInstance()->getInstance()->Log("Moving Door: " + oRemoteBody->getLabel());
                                                }
                                            }
                                        }
                                    }
                                }

                                if (!strcmp(classnameRemote, "trigger_counter") ) {
                                    // Si el objeto original era un botón
                                    if (!strcmp(classname, "func_button")) {
                                        Mesh3DBody *oButton = dynamic_cast<Mesh3DBody*> (brkObjectB);
                                        if (oButton->active) {
                                            for (int k = 0; k < this->gameObjects->size(); k++) {
                                                Mesh3D *oRemoteMesh = dynamic_cast<Mesh3D*> ((*this->gameObjects)[k]);
                                                if (oRemoteMesh != NULL) {
                                                    if (oRemoteMesh->getBspEntityIndex() == targetRemoteEntityId) {

                                                        Mesh3DGhost *oRemoteGhost = dynamic_cast<Mesh3DGhost*> (oRemoteMesh);
                                                        oRemoteGhost->currentTriggerCounter++;
                                                        oButton->active = false;
                                                        if (EngineSetup::getInstance()->LOG_COLLISION_OBJECTS) {
                                                            Logging::getInstance()->getInstance()->Log("trigger_counter for BSPEntity: " + std::to_string(targetRemoteEntityId) + "=" + std::to_string(oRemoteGhost->currentTriggerCounter));
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    } else {
                        if (EngineSetup::getInstance()->LOG_COLLISION_OBJECTS) {
                            Logging::getInstance()->getInstance()->Log("[AllPairs] Collision between " + brkObjectA->getLabel() + " and " + brkObjectB->getLabel());
                        }
                    }
                }
            }
        }
    }
}

btDiscreteDynamicsWorld *CollisionResolver::getDynamicsWorld() const {
    return dynamicsWorld;
}

void CollisionResolver::setDynamicsWorld(btDiscreteDynamicsWorld *dynamicsWorld) {
    CollisionResolver::dynamicsWorld = dynamicsWorld;
}

Camera3D *CollisionResolver::getCamera() const {
    return camera;
}

void CollisionResolver::setCamera(Camera3D *camera) {
    CollisionResolver::camera = camera;
}

Mesh3DGhost *CollisionResolver::getTriggerCamera() const {
    return triggerCamera;
}

void CollisionResolver::setTriggerCamera(Mesh3DGhost *triggerCamera) {
    CollisionResolver::triggerCamera = triggerCamera;
}

BSPMap *CollisionResolver::getBspMap() const {
    return bspMap;
}

void CollisionResolver::setBspMap(BSPMap *bspMap) {
    CollisionResolver::bspMap = bspMap;
}

void CollisionResolver::moveMesh3DBody(Mesh3DBody *oRemoteBody, int targetEntityId) {

    if ( oRemoteBody->isMoving()|| oRemoteBody->isReverseMoving() || oRemoteBody->isWaiting()) return;

    char *angle = bspMap->getEntityValue(targetEntityId, "angle");
    char *speed = bspMap->getEntityValue(targetEntityId, "speed");

    float angleFloat = atof( std::string(angle).c_str() );
    float speedFloat = atof( std::string(speed).c_str() );

    oRemoteBody->setMoving(true);
    oRemoteBody->setAngleMoving(angleFloat);

    if (speedFloat > 0) {
        oRemoteBody->setSpeedMoving(speedFloat);
    }
}

std::vector<Object3D *> *CollisionResolver::getGameObjects() const {
    return gameObjects;
}

void CollisionResolver::setGameObjects(std::vector<Object3D *> *gameObjects) {
    CollisionResolver::gameObjects = gameObjects;
}
