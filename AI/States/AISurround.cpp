#include "AISurround.h"
#include "../../AIEnemy.h"
#include "../../../GameLogic/Objects/Playership.h"
#include "../../../math/Rotation.h"
#include "../../../math/maths.h"
#include "../../../utility/RandomGenerator.h"

AISurround::AISurround()
{
    float da = RandomGenerator::GET_RANDOM_FLOAT(30.0f, 90.0f);
    m_angle = da * Math::DEG_TO_RAD;
}

void AISurround::update( float dt, Playership *player, AIEnemy *enemy )
{    
    Rotation r(Vector3(0.0f, 1.0f, 0.0f), m_angle);

    Vector3 dir = player->getPosition() - enemy->getEnemyPosition();
    dir.normalize();
    r.applyTo(dir);
    
    enemy->setEnemyThrusterDirection( dir );
}