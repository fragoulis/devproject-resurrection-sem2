#ifndef _RES_AICHASE_H_
#define _RES_AICHASE_H_
#include "IAIState.h"
 
class AIChase : public IAIState
{
public:
    virtual void update( float, Playership*, AIEnemy* );
};

#endif
