
#ifndef BRAKEZA3D_PLAYER_H
#define BRAKEZA3D_PLAYER_H

#include "../../headers/Objects/Object3D.h"
#include "../../headers/Misc/Counter.h"

typedef enum {LIVE, DEAD, GAMEOVER} PlayerState;

class Player {
private:

    float stamina;
    bool  dead;
    bool  stooped;
    int   lives;
    int   defaultLives;
    float air;

public:
    PlayerState state;

    Counter *counterStep;
    Counter *counterTakeDamage;

    bool tookDamage;

    Player();

    void newGame();
    void respawn();

    void shoot();

    void takeDamage(float dmg);

    int getStamina() const;
    void setStamina(int stamina);

    int getLives() const;
    void setLives(int lives);

    bool isDead() const;
    void setDead(bool dead);

    void evalStatusMachine();
    void respawnNPCS();

    bool isStooped() const;

    void setStooped(bool stooped);

    float getAir() const;
    void  setAir(float air);
};


#endif //BRAKEZA3D_PLAYER_H
