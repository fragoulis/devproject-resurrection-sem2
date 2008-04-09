#include "AIChase.h"
#include "../../AIEnemy.h"
#include "../../../GameLogic/Objects/Playership.h"
 
void AIChase::update( float dt, Playership *player, AIEnemy *enemy )
{
    // Get direction to the player ship
    Vector3 dir = player->getPosition() - enemy->getEnemyPosition();
    dir.normalize();
    enemy->setEnemyThrusterDirection( dir );
}