//
// Created by darkhead on 2019-05-11.
//

#include <btBulletDynamicsCommon.h>
#include "../../headers/Objects/Mesh3DBody.h"
#include "../../headers/Render/Logging.h"
#include "../../headers/Render/Transforms.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "../../headers/Render/Engine.h"

extern Engine *brakeza3D;

Mesh3DBody::Mesh3DBody()
{
    m_isActive = false;
    mass = 1.f;
    bsp_entity_index = -1;
    moving = false;
    reverseMoving = false;
    waiting = false;
    offsetMoving = 0;
    timer = new Timer();
}

btRigidBody* Mesh3DBody::makeRigidBody(float mass, std::vector<Mesh3DBody*> &physicsObjects, Camera3D *cam, btDiscreteDynamicsWorld *world, bool useObjectSpace)
{
    btConvexHullShape* me = new btConvexHullShape();

    for (int i=0; i < this->n_triangles; i++) {
        this->model_triangles[i].updateFullVertexSpaces(cam);
        btVector3 a, b, c;
        // Esto solo lo utilizamos para mayas procedentes de triángulos BSP en crudo.
        if (useObjectSpace) {
            a = btVector3( this->model_triangles[i].Ao.x, this->model_triangles[i].Ao.y, this->model_triangles[i].Ao.z );
            b = btVector3( this->model_triangles[i].Bo.x, this->model_triangles[i].Bo.y, this->model_triangles[i].Bo.z );
            c = btVector3( this->model_triangles[i].Co.x, this->model_triangles[i].Co.y, this->model_triangles[i].Co.z );
        } else {
            a = btVector3( this->model_triangles[i].A.x, this->model_triangles[i].A.y, this->model_triangles[i].A.z );
            b = btVector3( this->model_triangles[i].B.x, this->model_triangles[i].B.y, this->model_triangles[i].B.z );
            c = btVector3( this->model_triangles[i].C.x, this->model_triangles[i].C.y, this->model_triangles[i].C.z );
        }
        me->addPoint(a);
        me->addPoint(b);
        me->addPoint(c);
    }

    this->mass = mass;

    btTransform trans;
    trans.setIdentity();

    Vertex3D pos = *this->getPosition();
    trans.setOrigin(btVector3(pos.x , pos.y, pos.z));

    btVector3 localInertia(0, 0, 0);
    btDefaultMotionState* myMotionState = new btDefaultMotionState(trans);
    btCollisionShape* shape = new btConvexHullShape(*me);
    btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);
    this->m_body = new btRigidBody(cInfo);
    m_body->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);

    this->m_body->setUserPointer(this);

    world->addRigidBody(this->m_body);
    physicsObjects.push_back(this);


    return this->m_body;
}

void Mesh3DBody::integrate()
{
    if (m_isActive) {
        m_body->activate();
    }

    if (this->mass == 0) {
        //return;
    }

    if (this->isMoving() || this->isReverseMoving() || this->isWaiting()) {
        this->integrateMoving();
    }

    // Sync position
    btTransform t ;
    m_body->getMotionState()->getWorldTransform(t);
    btVector3 pos = t.getOrigin();

    Vertex3D worldPosition = Vertex3D(pos.getX() , pos.getY() , pos.getZ());
    this->setPosition(worldPosition);

    // Sync rotation
    btQuaternion  quat = t.getRotation();
    float angle = quat.getAngle();
    btVector3 axis = quat.getAxis();

}

void Mesh3DBody::integrateMoving()
{
    if (this->isWaiting()) {
        // Frame secuence control
        float deltatime = this->timer->getTicks() - this->last_ticks;
        this->last_ticks = this->timer->getTicks();
        this->timerCurrent += (deltatime/1000.f);

        if (this->timerCurrent >= waitTime) {
            timer->stop();
            this->setWaiting(false);
            this->setReverseMoving(true);
        }

        return;
    }

    float sizeX = this->aabbMax.x - this->aabbMin.x;
    float sizeY = this->aabbMax.y - this->aabbMin.y;
    float sizeZ = this->aabbMax.z - this->aabbMin.z;

    float speed = speedMoving / 25; // quake speed reductor for Brakeza3D

    // Sync position
    btTransform t;
    this->m_body->getMotionState()->getWorldTransform(t);
    btVector3 posBody = t.getOrigin();

    // horizontal move
    if (angleMoving >= 0) {
        M3 r = M3::getMatrixRotationForEulerAngles(0, -angleMoving, 0);
        Vertex3D right = r * EngineSetup::getInstance()->right;

        if (reverseMoving) {
            right = right.getInverse();
        }

        float directionSize = sizeZ;
        if (angleMoving == 90 || angleMoving == 270 ){
            directionSize = sizeZ;
        }
        if (angleMoving == 0 || angleMoving == 180 ){
            directionSize = sizeX;
        }

        // Si he superado el límite actualizo status
        if ( offsetMoving >= abs(directionSize) ) {

            if (reverseMoving == true) {
                reverseMoving = false;
            } else {
                timer->start();
                moving = false;
                reverseMoving = false;
                waiting = true;
            }

            offsetMoving = 0;
            return;
        }

        Vertex3D move = right.getScaled( brakeza3D->getDeltaTime() * speed );
        offsetMoving+=move.getModule();

        posBody[0] += move.x;
        posBody[1] += move.y;
        posBody[2] += move.z;
    }

    // vertical move down
    if (angleMoving == -2) {
        Vertex3D down = EngineSetup::getInstance()->down;

        if (reverseMoving) {
            down = down.getInverse();
        }

        if ( offsetMoving >= abs(sizeY) ) {
            if (reverseMoving == true) {
                reverseMoving = false;
            } else {
                timer->start();
                moving = false;
                reverseMoving = false;
                waiting = true;
            }

            offsetMoving = 0;
            return;
        }

        Vertex3D move = down.getScaled(brakeza3D->getDeltaTime() * speed);
        offsetMoving+=move.getModule();

        posBody[0] -= move.x;
        posBody[1] -= move.y;
        posBody[2] -= move.z;
    }

    t.setOrigin(posBody);
    this->m_body->setWorldTransform(t);
    this->m_body->getMotionState()->setWorldTransform(t);
}

bool Mesh3DBody::isMoving() const {
    return moving;
}

void Mesh3DBody::setMoving(bool moving) {
    Mesh3DBody::moving = moving;
}

bool Mesh3DBody::isReverseMoving() const {
    return reverseMoving;
}

void Mesh3DBody::setReverseMoving(bool moving) {
    Mesh3DBody::reverseMoving = moving;
}

void Mesh3DBody::setWaiting(bool waiting){
    Mesh3DBody::waiting = waiting;

}

bool Mesh3DBody::isWaiting() const {
    return waiting;
}

void Mesh3DBody::setSpeedMoving(float speed)
{
    this->speedMoving = speed;
}

float Mesh3DBody::getSpeedMoving()
{
    return this->speedMoving;
}

float Mesh3DBody::getAngleMoving() const {
    return angleMoving;
}

void Mesh3DBody::setAngleMoving(float angleMoving) {
    Mesh3DBody::angleMoving = angleMoving;
}
