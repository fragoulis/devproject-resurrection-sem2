//*****************************************************************************
//**
//**   File:               Point2.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#include "Point2.h"
#include <cmath>
#include <iostream>
using namespace std;

Point2::Point2(const float components[])
{
	set(components[0], components[1]);
}
Point2::Point2(const float x, const float y)
{
	set(x, y);
}

Point2::~Point2()
{
}

void Point2::set(const float x, const float y)
{
	setX(x);
	setY(y);
}

void Point2::add(const Vector2& v)
{
	m_vector.add(v);
}

Vector2 Point2::subtract(const Point2& p) const
{
	return m_vector - p.m_vector;
}
void Point2::subtract(const Vector2& v)
{
	m_vector -= v;
}

float Point2::distance(const Point2& p) const
{
	return float(sqrt(fabs(
		(getX() - p.getX()) * (getX() - p.getX()) +
		(getY() - p.getY()) * (getY() - p.getY())
		)));
}




istream& Point2::read(istream& is)
{
	return m_vector.read(is);
}
ostream& Point2::write(ostream& os) const
{
	return m_vector.write(os);
}
