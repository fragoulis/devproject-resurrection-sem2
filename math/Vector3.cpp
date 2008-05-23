#include "Vector3.h"
#include "Matrix33.h" 
#include <cmath>
#include <iostream>
using namespace std;

Vector3::Vector3(const float components[])
{
	set(components[0], components[1], components[2]);
}
Vector3::Vector3(const float x, const float y, const float z)
{
	set(x, y, z);
}

Vector3::~Vector3()
{
}

void Vector3::set(const float x, const float y, const float z)
{
	setX(x);
	setY(y);
	setZ(z);
}

void Vector3::add(const Vector3& v)
{
	add(v.getX(), v.getY(), v.getZ());
}
void Vector3::add(const float x, const float y, const float z)
{
	m_components[0] += x;
	m_components[1] += y;
	m_components[2] += z;
}

void Vector3::subtract(const Vector3& v)
{
	subtract(v.getX(), v.getY(), v.getZ());
}

void Vector3::subtract(const float x, const float y, const float z)
{
	m_components[0] -= x;
	m_components[1] -= y;
	m_components[2] -= z;
}

void Vector3::multiply(const float m)
{
	multiply(m, m, m);
}

void Vector3::multiply(const float x, const float y, const float z)
{
	m_components[0] *= x;
	m_components[1] *= y;
	m_components[2] *= z;
}

void Vector3::multiply(const Matrix33& m)
{
	float x = m_components[0], y = m_components[1], z = m_components[2];
	setX(x * m.get(0, 0) + y * m.get(0, 1) + z * m.get(0, 2));
	setY(x * m.get(1, 0) + y * m.get(1, 1) + z * m.get(1, 2));
	setZ(x * m.get(2, 0) + y * m.get(2, 1) + z * m.get(2, 2));
}

void Vector3::negate()
{
	setX(-getX());
	setY(-getY());
	setZ(-getZ());
}

float Vector3::length() const
{
	return float(sqrt(dot(*this)));
}
float Vector3 :: lengthSquared() const
{
	return dot(*this);
}


void Vector3::normalize()
{
	multiply(float(1.0f / length()));
}

float Vector3::dot(const Vector3& v) const
{
	return getX() * v.getX() + getY() * v.getY() + getZ() * v.getZ();
}

float Vector3::angle(const Vector3& v) const
{
	return acos(max(min(dot(v), 1.0f), -1.0f));
}

void Vector3::cross(const Vector3 &rhs)
{
	Vector3 result(getY() * rhs.getZ() - getZ() * rhs.getY(),
				   getZ() * rhs.getX() - getX() * rhs.getZ(),
				   getX() * rhs.getY() - getY() * rhs.getX());
	*this = result;
}

const Vector3 Vector3::cross(const Vector3& lhs, const Vector3& rhs) {
	Vector3 ret(lhs);
	ret.cross(rhs);
	return ret;
}



istream& Vector3::read(istream& is)
{
	char skip;
	ws(is);
	is >> skip >> m_components[0] >> skip >> m_components[1] >> skip >> m_components[2] >> skip;
	ws(is);
	return is;
}
ostream& Vector3::write(ostream& os) const
{
	os << " (" << int(m_components[0] * 10.0f) << ", " << int(m_components[1] * 10.0f) << ", " << int(m_components[2] * 10.0f) << ") / 10";
	return os;
}
