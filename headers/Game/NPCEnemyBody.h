
#ifndef BRAKEDA3D_NPCENEMYBODY_H
#define BRAKEDA3D_NPCENEMYBODY_H


#include "../Physics/SpriteDirectional3DBody.h"

typedef enum {ENEMY_STATE_STOP, ENEMY_STATE_FOLLOW, ENEMY_STATE_ATTACK, ENEMY_STATE_INJURIED, ENEMY_STATE_DIE} EnemyState;

class NPCEnemyBody: public SpriteDirectional3DBody, public Enemy {

public:
    EnemyState state;

    std::vector<Vertex3D> points;
    SpriteDirectional3D *projectileTemplate;

    float stepIA;
    Counter *counterIA;
    Counter *counterInjuried;

    NPCEnemyBody();

    void evalStatusMachine( bool raycastResult, float raycastlength, Camera3D *cam, btDiscreteDynamicsWorld *dynamicsWorld, std::vector<Object3D*> &gameObjects);

    void doFollowPathfinding(bool raycastResult);
    void syncPathFindingRotation();
    void shoot(Camera3D *cam, btDiscreteDynamicsWorld *dynamicsWorld, std::vector<Object3D*> &gameObjects);

    void respawn();

    void updateCounters();
};


#endif //BRAKEDA3D_NPCENEMYBODY_H
