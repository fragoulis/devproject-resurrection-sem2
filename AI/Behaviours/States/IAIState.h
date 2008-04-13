#pragma once

class Playership;
class AIEnemy;

/**
 * A state is independent of any enemy.
 * This means that for any number of enemies only one instance
 * of each behaviour will exist.
 */

class IAIState
{
public:
    virtual ~IAIState() = 0 {};
    virtual void update( float, Playership*, AIEnemy* ) = 0;
    virtual void onBegin(AIEnemy*) {}
    virtual void onEnd(AIEnemy*) {}
};