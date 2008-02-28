#include "Vector2.h"
#include <cmath>
#include <iostream>
using namespace std;

Vector2::Vector2(const float components[])
{
	set(components[0], components[1]);
}
Vector2::Vector2(const float x, const float y)
{
	set(x, y);
}

Vector2::~Vector2()
{
}

void Vector2::set(const float x, const float y)
{
	setX(x);
	setY(y);
}

void Vector2::add(const Vector2& v)
{
	add(v.getX(), v.getY());
}
void Vector2::add(const float x, const float y)
{
	m_components[0] += x;
	m_components[1] += y;
}

void Vector2::subtract(const Vector2& v)
{
	subtract(v.getX(), v.getY());
}

void Vector2::subtract(const float x, const float y)
{
	m_components[0] -= x;
	m_components[1] -= y;
}

void Vector2::multiply(const float m)
{
	multiply(m, m);
}

void Vector2::multiply(const float x, const float y)
{
	m_components[0] *= x;
	m_components[1] *= y;
}

void Vector2::negate()
{
	setX(-getX());
	setY(-getY());
}

float Vector2::length() const
{
	return float(sqrt(dot(*this)));
}

void Vector2::normalize()
{
	multiply(float(1.0 / length()));
}

float Vector2::dot(const Vector2& v) const
{
	return getX() * v.getX() + getY() * v.getY();
}

float Vector2::angle(const Vector2& v) const
{
	return acos(max(min(dot(v), 1.0f), -1.0f));
}


istream& Vector2::read(istream& is)
{
	char skip;
	ws(is);
	is >> skip >> m_components[0] >> skip >> m_components[1] >> skip;
	ws(is);
	return is;
}
ostream& Vector2::write(ostream& os) const
{
	os << '(' << m_components[0] << ", " << m_components[1] << ')';
	return os;
}
