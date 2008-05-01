#include "InterceptorBuffCouple.h"
#include "../../utility/EventManager.h"
#include "../Enemies/Enemyship.h"
#include "../GameEvents.h"

InterceptorBuffCouple::InterceptorBuffCouple( Enemyship *enemyship, const Buff *buff ):
EnemyBuffCouple(enemyship,buff)
{}

void InterceptorBuffCouple::onCreate()
{
    // Fire interceptor clamped event
	EventManager::instance().fireEvent(Interceptor_Clamped(m_enemyship));
}

void InterceptorBuffCouple::onDestroy()
{
    assert(m_enemyship);

	// enemy ship might be invalid already
	// this is one way to fix this bug. Other fix is int coupleman
    if (!m_enemyship->isToBeDeleted())
	{
		// Fire destroyed event
		m_enemyship->setToBeDeleted();
		EventManager::instance().fireEvent(Enemy_Destroyed(m_enemyship));
	}
}