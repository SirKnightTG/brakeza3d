//
// Created by darkhead on 2019-08-07.
//

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include "../../headers/Physics/SpriteDirectional3DBody.h"
#include "../../headers/Render/Transforms.h"
#include "../../headers/Render/Engine.h"

SpriteDirectional3DBody::SpriteDirectional3DBody()
{
}

void SpriteDirectional3DBody::integrate()
{
    if (this->mass == 0) {
        //return;
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

btRigidBody* SpriteDirectional3DBody::makeRigidBody(float mass, std::vector<SpriteDirectional3DBody*> &projectilePhysics, Camera3D *cam, btDiscreteDynamicsWorld *world, bool applyCameraImpulse, int forceImpulse)
{
    this->mass = mass;

    btTransform trans;
    trans.setIdentity();

    Vertex3D pos = *this->getPosition();

    Vertex3D dir = cam->getRotation().getTranspose() * AxisForward();
    pos = pos + dir.getScaled(10);

    trans.setOrigin(btVector3(pos.x , pos.y, pos.z));

    btVector3 localInertia(0, 0, 0);

    btDefaultMotionState* myMotionState = new btDefaultMotionState(trans);
    //btCollisionShape* shape = new btSphereShape(1.0f);
    btCollisionShape* shape;
    if (this->mass == 0) {
        shape = new btBoxShape(btVector3(2.5, 2.5, 2.5));
    } else {
        shape = new btBoxShape(btVector3(0.5, 0.5, 0.5));
    }

    btRigidBody::btRigidBodyConstructionInfo cInfo(this->mass, myMotionState, shape, localInertia);
    this->m_body = new btRigidBody(cInfo);
    this->m_body->setUserPointer(this);
    this->m_body->setCcdMotionThreshold(0.01f);
    this->m_body->setCcdSweptSphereRadius(0.02f);

    if (applyCameraImpulse) {
        dir = dir.getScaled(forceImpulse);
        btVector3 impulse(dir.x, dir.y, dir.z);
        this->m_body->applyCentralImpulse(impulse);
    }

    if (this->mass == 0) {
    } else {
    }

    //this->m_body->setCollisionFlags( btCollisionObject::CF_NO_CONTACT_RESPONSE);

    world->addRigidBody(this->m_body);

    projectilePhysics.push_back(this);

    return this->m_body;
}
