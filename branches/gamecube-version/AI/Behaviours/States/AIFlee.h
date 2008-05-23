#ifndef _RES_AIFLEE_H_
#define _RES_AIFLEE_H_
#include "IAIState.h"
 
class AIFlee : public IAIState
{
public:
    virtual void update( float, Playership*, AIEnemy* );
};

#endif
