#include "AIFlee.h"
#include "../../AIEnemy.h"
#include "../../../GameLogic/Objects/Playership.h"
 
void AIFlee::update( float dt, Playership *player, AIEnemy *enemy )
{
    // Get direction to the player ship
    Vector3 dir = player->getPosition() - enemy->getEnemyPosition();
    dir.normalize();
    enemy->setEnemyThrusterDirection( dir );
}