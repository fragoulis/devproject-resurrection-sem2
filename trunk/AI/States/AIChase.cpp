#include "AIChase.h"
#include "../AIStateManager.h"
#include "../AIStateEnemyCouple.h"
#include "../../GameLogic/Objects/Playership.h"

void AIChase::update( Playership *player, AIStateEnemyCouple *couple, float dt )
{
    // Get direction to the player ship
    Vector3 dir = player->getPosition() - couple->getEnemyPosition();
    dir.normalize();
    couple->setEnemyThrusterDirection( dir );

    if( couple->readyToChange(dt) )
    {    
        couple->setState( AIStateManager::instance().getState("Surround") );
    }
}