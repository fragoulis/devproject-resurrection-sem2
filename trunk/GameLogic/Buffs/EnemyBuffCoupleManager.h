#pragma once
#include "../../utility/Singleton.h"
#include "../../GameLogic/GameEvents.h"
#include "../../utility/EventManager.h"
#include <list>
class Buff;
class Enemyship;
class EnemyBuffCouple;


/**
 * Manages EnemyBuff couples.
 * It will fire onDestroy when the buff expires.
 */
class EnemyBuffCoupleManager : 
    public Singleton<EnemyBuffCoupleManager>,
    public EventListener< Player_Respawned >,
    public EventListener< Level_Unload >
{
private:
    typedef std::list<EnemyBuffCouple*> EnemyBuffCouples;
    EnemyBuffCouples m_enemyBuffCouples;

public:
    void update();
    void addInterceptorBuffCouple( Enemyship *enemyship, const Buff *buff );

    void onEvent(Player_Respawned&);
    void onEvent(Level_Unload&);

private:
    friend Singleton<EnemyBuffCoupleManager>;
    EnemyBuffCoupleManager();
    ~EnemyBuffCoupleManager();

    void _addEnemyBuffCouple( EnemyBuffCouple *enemyBuffCouple );
};