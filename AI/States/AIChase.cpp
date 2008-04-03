#include "AIChase.h"
#include "../../GameLogic/Objects/Playership.h"
#include "../../GameLogic/Enemies/Enemyship.h"

void AIChase::update( Playership *player, Enemyship *enemy ) const
{
    // Get direction to the player ship
    Vector3 dir = player->getPosition() - enemy->getPosition();
    dir.normalize();
    enemy->setThrusterDirection( dir );
}