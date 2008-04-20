#pragma once
#include "Buff.h"
#include "../Enemies/Enemyship.h"

class EnemyBuffCouple
{
protected:
    Enemyship *m_enemyship;
    const Buff *m_buff;
    
public:
    EnemyBuffCouple( Enemyship *enemyship, const Buff *buff );
    virtual ~EnemyBuffCouple() {}

    bool isBuffActive() const { return m_buff->isActive(); }

    // Override these
    virtual void onCreate() = 0;
    virtual void onDestroy() = 0;
};