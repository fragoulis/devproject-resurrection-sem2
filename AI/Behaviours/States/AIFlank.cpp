#include "AIFlank.h"
#include "../../AIEnemy.h"
#include "../../../GameLogic/Objects/Playership.h"
#include "../../../math/maths.h"

void AIFlank::update( float dt, Playership *player, AIEnemy *enemy )
{
    // Get direction to the player ship
    Vector3 dir = player->getPosition() - enemy->getEnemyPosition();
    dir.normalize();

    const Vector3 &player_dir = player->getThrusterDirection();
    const float dot = player_dir.dot( dir );

    // if enemy is ahead of the player
    if( dot > 0.2f )
    {
        enemy->setEnemyThrusterDirection( player_dir );
    }
    else
    {
        enemy->setForceChange();
    }
}