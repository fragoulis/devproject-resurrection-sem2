#ifndef _RES_AISURROUND_H_
#define _RES_AISURROUND_H_
#include "IAIState.h"

class AISurround : public IAIState
{
private:
    float m_angle;

public:
    AISurround();
    virtual void update( float, Playership*, AIEnemy* );
};


#endif
