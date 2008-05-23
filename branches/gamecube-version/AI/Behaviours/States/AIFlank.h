#ifndef _RES_AIFLANK_H_
#define _RES_AIFLANK_H_
#include "IAIState.h"
 
/**
 * Flank state has the same effect as Chase state
 * but also checks for enemy fire direction in order to 
 * try to avoid the line of fire.
 */

class AIFlank : public IAIState
{
public:
    virtual void update( float, Playership*, AIEnemy* );
};

#endif
