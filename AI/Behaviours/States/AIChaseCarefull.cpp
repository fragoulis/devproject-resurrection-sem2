#include "AIChaseCarefull.h"
#include "../../AIEnemy.h"
#include "../../../GameLogic/Objects/Playership.h"
 
void AIChaseCarefull::update( float dt, Playership *player, AIEnemy *enemy )
{
    // Get direction to the player ship
    Vector3 dir = player->getPosition() - enemy->getEnemyPosition();
    dir.normalize();

    const Vector3 &player_dir = player->getThrusterDirection();
    const float per = player_dir.dot( dir );

    const Rotation r(Vector3(0.0f, 1.0f, 0.0f), per);
    r.applyTo(dir);


    enemy->setEnemyThrusterDirection( dir );
}