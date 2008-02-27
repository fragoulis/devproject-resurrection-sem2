#include "Rigidbody.h"

Rigidbody :: Rigidbody()
{
}

Rigidbody :: ~Rigidbody()
{
}

void Rigidbody :: setInertia( const Matrix33& i )
{
	m_inertia = i;
	m_inertiaInverse = i;
	m_inertiaInverse.invert();
}