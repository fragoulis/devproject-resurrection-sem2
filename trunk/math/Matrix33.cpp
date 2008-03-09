//*****************************************************************************
//**
//**   File:               Matrix33.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#include "Matrix33.h"

Matrix33::Matrix33()
{
}

Matrix33::Matrix33(float f)
{
	for (int i = 0; i < 9; i++) m_components[i] = 0.0f;
	for (int i = 0; i < 3; i++) m_components[i*4] = f;
}

Matrix33::Matrix33(const float components[9])
{
	for (int i = 0; i < 9; i++)
		m_components[i] = components[i];
}

Matrix33::Matrix33(const float f11, const float f21, const float f31,
				   const float f12, const float f22, const float f32,
				   const float f13, const float f23, const float f33)
{
	set(f11, f21, f31, f12, f22, f32, f13, f23, f33);
}

Matrix33::~Matrix33()
{
}

void Matrix33::loadIdentity()
{
	for (int i = 0; i < 9; i++) m_components[i] = 0.0f;
	for (int i = 0; i < 3; i++) m_components[i*4] = 1.0f;
}
void Matrix33::add(const Matrix33& m)
{
	for (int i = 0; i < 9; i++) m_components[i] += m.m_components[i];
}

void Matrix33::multiply(const float f)
{
	for (int i = 0; i < 9; i++) m_components[i] *= f;
}

void Matrix33 :: set(float f11, float f21, float f31,
		             float f12, float f22, float f32,
			         float f13, float f23, float f33)
{
	m_components[0] = f11; m_components[3] = f21; m_components[6] = f31;
	m_components[1] = f12; m_components[4] = f22; m_components[7] = f32;
	m_components[2] = f13; m_components[5] = f23; m_components[8] = f33;
}


void Matrix33::postMultiply(const Matrix33& m)
{
	//for (int i = 0; i < 4; i++) {
	//	for (int j = 0; j < 4; j++) {
	//		float& f = m_components[i + j * 4] = 0;
	//		for (int l = 0; l < 4; l++) {
	//			f += get(i, l) * m.get(l, j);
	//		}
	//	}
	//}
	float n[3][3];

	n[0][0] = get(0, 0) * m.get(0, 0) + get(0, 1) * m.get(1, 0) + get(0, 2) * m.get(2, 0);
	n[0][1] = get(0, 0) * m.get(0, 1) + get(0, 1) * m.get(1, 1) + get(0, 2) * m.get(2, 1);
	n[0][2] = get(0, 0) * m.get(0, 2) + get(0, 1) * m.get(1, 2) + get(0, 2) * m.get(2, 2);
	n[1][0] = get(1, 0) * m.get(0, 0) + get(1, 1) * m.get(1, 0) + get(1, 2) * m.get(2, 0);
	n[1][1] = get(1, 0) * m.get(0, 1) + get(1, 1) * m.get(1, 1) + get(1, 2) * m.get(2, 1);
	n[1][2] = get(1, 0) * m.get(0, 2) + get(1, 1) * m.get(1, 2) + get(1, 2) * m.get(2, 2);
	n[2][0] = get(2, 0) * m.get(0, 0) + get(2, 1) * m.get(1, 0) + get(2, 2) * m.get(2, 0);
	n[2][1] = get(2, 0) * m.get(0, 1) + get(2, 1) * m.get(1, 1) + get(2, 2) * m.get(2, 1);
	n[2][2] = get(2, 0) * m.get(0, 2) + get(2, 1) * m.get(1, 2) + get(2, 2) * m.get(2, 2);

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			set(i, j, n[i][j]);
}

float Matrix33 :: determinant() const
{
	return get(1, 1) * get(2, 2) * get(3, 3) -
		   get(1, 1) * get(3, 2) * get(2, 3) +
		   get(2, 1) * get(3, 2) * get(1, 3) -
		   get(2, 1) * get(1, 2) * get(3, 3) +
		   get(3, 1) * get(1, 2) * get(2, 3) -
		   get(3, 1) * get(2, 2) * get(1, 3);
}

void Matrix33 :: invert()
{
	float d = determinant();
	// test is d is 0?

	set( get(2,2)*get(3,3)-get(2,3)*get(3,2),
		 get(1,3)*get(3,2)-get(1,2)*get(3,3),
		 get(1,2)*get(2,3)-get(1,3)*get(2,2),
		 get(2,3)*get(3,1)-get(2,1)*get(3,3),
		 get(1,1)*get(3,3)-get(1,3)*get(3,1),
		 get(1,3)*get(2,1)-get(1,1)*get(2,3),
		 get(2,1)*get(3,2)-get(2,2)*get(3,1),
		 get(1,2)*get(3,1)-get(1,1)*get(3,2),
		 get(1,1)*get(2,2)-get(1,2)*get(2,1) );
	multiply(1/d);
}
