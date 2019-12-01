
#include "../../headers/Misc/Counter.h"
#include "../../headers/Brakeza3D.h"

Counter::Counter() {
    this->timer = Brakeza3D::get()->getTimer();
}

Counter::Counter(float step) {
    this->timer = Brakeza3D::get()->getTimer();
    this->step = step;
}

void Counter::update()
{
    if (this->timer == NULL) return;

    float ticks     = this->timer->getTicks();
    float deltatime = ticks - this->lastTicks;
    this->lastTicks = ticks;

    acumulatedTime += (deltatime / 1000);

    if (acumulatedTime >= step) {
        resetAcumulated();
        finished = true;
        return;
    }

    finished = false;
}

bool Counter::isFinished() const {
    return finished;
}

void Counter::setTimer(Timer *timer) {
    Counter::timer = timer;
}

void Counter::setStep(float step) {
    this->step = step;
    resetAcumulated();
}

void Counter::resetAcumulated() {
    this->acumulatedTime = 0;
}

float Counter::getAcumulatedTime() const {
    return acumulatedTime;
}

float Counter::getStep() const {
    return step;
}

