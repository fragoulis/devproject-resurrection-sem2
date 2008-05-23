#ifndef _RES_INTERCEPTORBUFFCOUPLE_H_
#define _RES_INTERCEPTORBUFFCOUPLE_H_
#include "EnemyBuffCouple.h"


/**
 * An implementation of the BuffCouple.
 * Fires the Interceptor_Clamped event when created.
 * Destroys the interceptor when destroyed.
 */

class InterceptorBuffCouple : public EnemyBuffCouple
{
public:
    InterceptorBuffCouple( Enemyship *enemyship, const Buff *buff );

    virtual void onCreate();
    virtual void onDestroy();
};

#endif
