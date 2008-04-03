#include "AISurround.h"
#include "../AIStateManager.h"
#include "../AIStateEnemyCouple.h"
#include "../../GameLogic/Objects/Playership.h"
#include "../../math/Rotation.h"
#include "../../utility/RandomGenerator.h"
#include "../../math/maths.h"

AISurround::AISurround()
{
    float da = RandomGenerator::GET_RANDOM_FLOAT(30.0f, 90.0f);
    m_angle = da * Math::DEG_TO_RAD;
}

void AISurround::update( Playership *player, AIStateEnemyCouple *couple, float dt )
{    
    Rotation r(Vector3(0.0f, 1.0f, 0.0f), m_angle);

    Vector3 dir = player->getPosition() - couple->getEnemyPosition();
    dir.normalize();
    r.applyTo(dir);
    
    couple->setEnemyThrusterDirection( dir );

    if( couple->readyToChange(dt) )
    {    
        couple->setState( AIStateManager::instance().getState("Chase") );
    }
}