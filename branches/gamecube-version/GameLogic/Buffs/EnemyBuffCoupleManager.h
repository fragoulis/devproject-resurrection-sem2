#ifndef _RES_ENEMYBUFFCOUPLEMANAGER_H_
#define _RES_ENEMYBUFFCOUPLEMANAGER_H_
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
    public EventListener< Level_Unload >,
	public EventListener< Enemy_Despawned >
{
private:
    typedef std::list<EnemyBuffCouple*> EnemyBuffCouples;
    EnemyBuffCouples m_enemyBuffCouples;

public:
    void update();
    void addInterceptorBuffCouple( Enemyship *enemyship, const Buff *buff );

    virtual void onEvent(Player_Respawned&);
    virtual void onEvent(Level_Unload&);
	virtual void onEvent(Enemy_Despawned&);

private:
    friend Singleton<EnemyBuffCoupleManager>;
    EnemyBuffCoupleManager();
    ~EnemyBuffCoupleManager();

    void _addEnemyBuffCouple( EnemyBuffCouple *enemyBuffCouple );
	void _removeEnemy( Enemyship* enemyship );
};

#endif
