
#ifndef BRAKEDA3D_ENEMY_H
#define BRAKEDA3D_ENEMY_H


#include <vector>
#include "Vertex3D.h"
#include "../Physics/Body.h"


class Enemy {
public:
    std::string classname;  // For BSP classname match

    float startStamina;
    float stamina;

    bool dead;

    float damage;   // Damage from enemy to others

    float cadence;  // cadence management
    Timer cadenceTimer;
    float lastTicks;
    float acumulatedTime = 0;

    float range;
    float speed;

    Enemy();

    void takeDamage(float damageTaken);
    bool isTakeHeavyDamage(float damageTaken);

    float getDamage() const;
    void setDamage(float damage);

    float getRange() const;
    void setRange(float range);

    float getSpeed() const;
    void setSpeed(float speed);

    float getCadence() const;
    void setCadence(float cadence);

    const std::string &getClassname() const;
    void setClassname(const std::string &classname);

    bool isDead() const;
    void setDead(bool dead);

    void startFire();
    void endFire();
    void updateCadenceTimer();
    bool isCadenceInProgress();

};


#endif //BRAKEDA3D_ENEMY_H
