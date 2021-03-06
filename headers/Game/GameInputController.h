
#ifndef BRAKEZA3D_GAMEINPUTCONTROLLER_H
#define BRAKEZA3D_GAMEINPUTCONTROLLER_H

#include "../Input/InputController.h"
#include "../../src/Game/Player.h"

class GameInputController : public InputController
{
public:
    Player *player;

    GameInputController(Player *player);

    void handleKeyboardContinuous(SDL_Event *, bool &);
    void handleKeyboard(SDL_Event *event, bool &end);
    void handleMouse(SDL_Event *);

    void handleSniper(SDL_Event *event);
    void handleCrouch(SDL_Event *event);
    void handleFire(SDL_Event *event);
    void handleWeaponSelector(SDL_Event *event);
    void handleZoom(SDL_Event *event);

    void handleMenuKeyboard(bool &end);

    void jump(bool checkOnGround, float YForce, bool soundJump);

};


#endif //BRAKEZA3D_GAMEINPUTCONTROLLER_H
