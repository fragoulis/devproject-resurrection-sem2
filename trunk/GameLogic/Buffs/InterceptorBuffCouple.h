#include "EnemyBuffCouple.h"

class InterceptorBuffCouple : public EnemyBuffCouple
{
public:
    InterceptorBuffCouple( Enemyship *enemyship, const Buff *buff );

    virtual void onCreate();
    virtual void onDestroy();
};