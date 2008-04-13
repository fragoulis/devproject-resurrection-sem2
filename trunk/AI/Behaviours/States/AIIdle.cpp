#include "AIIdle.h"
#include "../../AIEnemy.h"
#include "../../../GameLogic/Objects/Playership.h"

void AIIdle::update( float dt, Playership *player, AIEnemy *enemy )
{    
    // Get direction to the player ship
    Vector3 dir = player->getPosition() - enemy->getEnemyPosition();
    dir.normalize();

    enemy->setEnemyThrusterDirection( dir );
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