#ifndef _RES_ENEMYBUFFCOUPLE_H_
#define _RES_ENEMYBUFFCOUPLE_H_
#include "Buff.h"
#include "../Enemies/Enemyship.h"



/**
 * A pair of an enemy and a buff.
 *
 */
class EnemyBuffCouple
{
protected:
    Enemyship *m_enemyship;
    const Buff *m_buff;
    
public:
    EnemyBuffCouple( Enemyship *enemyship, const Buff *buff );
    virtual ~EnemyBuffCouple() {}

    bool isBuffActive() const { return m_buff->isActive(); }

	const Enemyship* getEnemyship() const { return m_enemyship; }

    // Override these
    virtual void onCreate() = 0;
    virtual void onDestroy() = 0;
};

#endif
