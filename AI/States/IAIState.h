#pragma once

class Playership;
class AIStateEnemyCouple;

class IAIState
{
public:
    virtual ~IAIState() = 0 {};
    virtual void update( Playership*, AIStateEnemyCouple*, float ) = 0;
};