#pragma once
#include "IAIState.h"

class AIChase : public IAIState
{
public:
    virtual void update( Playership *player, Enemyship *enemy ) const;
};