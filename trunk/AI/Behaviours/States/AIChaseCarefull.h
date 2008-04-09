#pragma once
#include "IAIState.h"
 
/**
 * ChaseCarefull state has the same effect as Chase state
 * but also checks for enemy fire direction in order to 
 * try to avoid the line of fire.
 */

class AIChaseCarefull : public IAIState
{
public:
    virtual void update( float, Playership*, AIEnemy* );
};