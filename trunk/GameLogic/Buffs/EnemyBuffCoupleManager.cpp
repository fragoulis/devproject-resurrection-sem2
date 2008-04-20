#include "EnemyBuffCoupleManager.h"
#include "InterceptorBuffCouple.h"
#include "../../utility/deleters.h"


EnemyBuffCoupleManager::EnemyBuffCoupleManager()
{
    EventManager::instance().registerEventListener< Player_Respawned >(this);
    EventManager::instance().registerEventListener< Level_Unload >(this);
}

EnemyBuffCoupleManager::~EnemyBuffCoupleManager()
{
    deleteList(m_enemyBuffCouples);
}

void EnemyBuffCoupleManager::addInterceptorBuffCouple( Enemyship *enemyship, const Buff *buff )
{
    InterceptorBuffCouple *couple = new InterceptorBuffCouple( enemyship, buff );
    _addEnemyBuffCouple( couple );
}

/**
 * Check if any buff has expired, if so remove it from the list
 */
void EnemyBuffCoupleManager::update()
{
    EnemyBuffCouples::iterator i = m_enemyBuffCouples.begin();
    for(; i != m_enemyBuffCouples.end(); )
    {
        EnemyBuffCouple *couple = *i;
        if( !couple->isBuffActive() )
        {
            couple->onDestroy();

            delete couple; couple = 0;
            i = m_enemyBuffCouples.erase(i);
        }
        else
        {
            ++i;
        }
    }
}

void EnemyBuffCoupleManager::_addEnemyBuffCouple( EnemyBuffCouple *enemyBuffCouple )
{
    enemyBuffCouple->onCreate();
    m_enemyBuffCouples.push_back( enemyBuffCouple );
}

void EnemyBuffCoupleManager::onEvent( Player_Respawned& evt )
{
	deleteList(m_enemyBuffCouples);
}

void EnemyBuffCoupleManager::onEvent( Level_Unload& evt )
{
    deleteList(m_enemyBuffCouples);
}