#include "AISurround.h"
#include "../../GameLogic/Objects/Playership.h"
#include "../../GameLogic/Enemies/Enemyship.h"
#include "../../math/Rotation.h"
#define _USE_MATH_DEFINES
#include <cmath>

const float DEG_TO_RAD = (float)M_PI / 180.0f;

AISurround::AISurround()
{
    m_angle = DEG_TO_RAD*45.0f;
}

void AISurround::update( Playership *player, Enemyship *enemy ) const
{    
    Rotation r(Vector3(0.0f, 1.0f, 0.0f), m_angle);

    Vector3 dir = player->getPosition() - enemy->getPosition();
    dir.normalize();
    r.applyTo(dir);
    
    enemy->setThrusterDirection( dir );
}