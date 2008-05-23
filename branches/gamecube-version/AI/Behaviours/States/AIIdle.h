#ifndef _RES_AIIDLE_H_
#define _RES_AIIDLE_H_
#include "IAIState.h"

class AIIdle : public IAIState
{
public:
    virtual void update( float, Playership*, AIEnemy* );
    virtual void onBegin(AIEnemy*);
    virtual void onEnd(AIEnemy*);
};


#endif
