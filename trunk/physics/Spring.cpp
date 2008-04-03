#include "Spring.h"
#include "../GameLogic/Spaceship.h"
#include <cassert>

Spring::Spring( spring_obj_t *a, spring_obj_t *b, float restLength )
{
    m_objects[0] = a;
    m_objects[1] = b;
    m_Ks = 100.0f;
    m_Kd = 100.0f;
    m_restLength = restLength;
}

Spring::Spring()
{
    m_Ks = 1.0f;
    m_Kd = 1.0f;
    m_restLength = 1.0f;
}

void Spring::compute()
{
    assert(m_objects[0]&&m_objects[1]);
    // Current length
    const Vector3 dist = m_objects[1]->getPosition() - m_objects[0]->getPosition();
    const float length = dist.length();
    const Vector3 normal = dist / length;

    // Spring force
    float spring = m_Ks * ( length - m_restLength );

    // Current velocities
    float vel1 = normal.dot( m_objects[0]->getVelocity() );
    float vel2 = normal.dot( m_objects[1]->getVelocity() );

    // Damping force
    float damp = m_Kd * ( vel1 - vel2 );

    // Total spring-damper force
    Vector3 total = ( spring - damp ) * normal;

    // Apply to particles
    m_objects[0]->addForce( total );
    total.negate();
    m_objects[1]->addForce( total );
}