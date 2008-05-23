#ifndef _RES_IAISTATE_H_
#define _RES_IAISTATE_H_

class Playership;
class AIEnemy;

/**
 * A state is independent of any enemy.
 * This means that for any number of enemies only one instance
 * of each behaviour will exist.
 */

class IAIState
{
public:
	virtual ~IAIState() { }
    virtual void update( float, Playership*, AIEnemy* ) = 0;
    virtual void onBegin(AIEnemy*) { }
    virtual void onEnd(AIEnemy*) { }
};

#endif
