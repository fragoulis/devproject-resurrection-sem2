#pragma once
#include "IAIState.h"
 
class AIFlee : public IAIState
{
public:
    virtual void update( float, Playership*, AIEnemy* );
};