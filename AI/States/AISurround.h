#pragma once
#include "IAIState.h"

class AISurround : public IAIState
{
private:
    float m_angle;

public:
    AISurround();
    virtual void update( Playership*, AIStateEnemyCouple*, float );
};
