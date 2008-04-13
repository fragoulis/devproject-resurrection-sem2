#include "AIIdle.h"
#include "../../AIEnemy.h"

void AIIdle::update( float dt, Playership *player, AIEnemy *enemy )
{    
    /* do nothing */
}

void AIIdle::onBegin(AIEnemy *enemy)
{
    enemy->setPreviousThrusterPower( enemy->getEnemyThrusterPower() );
    enemy->setEnemyThrusterPower(0.0f);
}

void AIIdle::onEnd(AIEnemy *enemy)
{
    enemy->setEnemyThrusterPower( enemy->getPreviousThrusterPower() );
}