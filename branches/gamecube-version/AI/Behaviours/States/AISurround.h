#pragma once
#include "IAIState.h"

class AISurround : public IAIState
{
private:
    float m_angle;

public:
    AISurround();
    virtual void update( float, Playership*, AIEnemy* );
};
