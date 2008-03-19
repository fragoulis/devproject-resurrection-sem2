//*****************************************************************************
//**
//**   File:               Matrix44.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#include "Matrix44.h"

Matrix44::Matrix44()
{
}

Matrix44::Matrix44(float f)
{
	for (int i = 0; i < 16; i++) m_components[i] = 0.0f;
	for (int i = 0; i < 4; i++) m_components[i*5] = f;
}

Matrix44::Matrix44(const float components[16])
{
	for (int i = 0; i < 16; i++)
		m_components[i] = components[i];
}

Matrix44::Matrix44(const float f11, const float f21, const float f31, const float f41, 
				   const float f12, const float f22, const float f32, const float f42, 
				   const float f13, const float f23, const float f33, const float f43, 
				   const float f14, const float f24, const float f34, const float f44)
{
	m_components[0] = f11; m_components[4] = f21; m_components[8] = f31; m_components[12] = f41;
	m_components[1] = f12; m_components[5] = f22; m_components[9] = f32; m_components[13] = f42;
	m_components[2] = f13; m_components[6] = f23; m_components[10] = f33; m_components[14] = f43;
	m_components[3] = f14; m_components[7] = f24; m_components[11] = f34; m_components[15] = f44;
}

Matrix44::~Matrix44()
{
}

void Matrix44::loadIdentity()
{
	for (int i = 0; i < 16; i++) m_components[i] = 0.0f;
	for (int i = 0; i < 4; i++) m_components[i*5] = 1.0f;
}
void Matrix44::add(const Matrix44& m)
{
	for (int i = 0; i < 16; i++) m_components[i] += m.m_components[i];
}

void Matrix44::multiply(const float f)
{
	for (int i = 0; i < 16; i++) m_components[i] *= f;
}

void Matrix44::postMultiply(const Matrix44& m)
{
	//for (int i = 0; i < 4; i++) {
	//	for (int j = 0; j < 4; j++) {
	//		float& f = m_components[i + j * 4] = 0;
	//		for (int l = 0; l < 4; l++) {
	//			f += get(i, l) * m.get(l, j);
	//		}
	//	}
	//}
	float n[4][4];

	n[0][0] = get(0, 0) * m.get(0, 0) + get(0, 1) * m.get(1, 0) + get(0, 2) * m.get(2, 0) + get(0, 3) * m.get(3, 0);
	n[0][1] = get(0, 0) * m.get(0, 1) + get(0, 1) * m.get(1, 1) + get(0, 2) * m.get(2, 1) + get(0, 3) * m.get(3, 1);
	n[0][2] = get(0, 0) * m.get(0, 2) + get(0, 1) * m.get(1, 2) + get(0, 2) * m.get(2, 2) + get(0, 3) * m.get(3, 2);
	n[0][3] = get(0, 0) * m.get(0, 3) + get(0, 1) * m.get(1, 3) + get(0, 2) * m.get(2, 3) + get(0, 3) * m.get(3, 3);
	n[1][0] = get(1, 0) * m.get(0, 0) + get(1, 1) * m.get(1, 0) + get(1, 2) * m.get(2, 0) + get(1, 3) * m.get(3, 0);
	n[1][1] = get(1, 0) * m.get(0, 1) + get(1, 1) * m.get(1, 1) + get(1, 2) * m.get(2, 1) + get(1, 3) * m.get(3, 1);
	n[1][2] = get(1, 0) * m.get(0, 2) + get(1, 1) * m.get(1, 2) + get(1, 2) * m.get(2, 2) + get(1, 3) * m.get(3, 2);
	n[1][3] = get(1, 0) * m.get(0, 3) + get(1, 1) * m.get(1, 3) + get(1, 2) * m.get(2, 3) + get(1, 3) * m.get(3, 3);
	n[2][0] = get(2, 0) * m.get(0, 0) + get(2, 1) * m.get(1, 0) + get(2, 2) * m.get(2, 0) + get(2, 3) * m.get(3, 0);
	n[2][1] = get(2, 0) * m.get(0, 1) + get(2, 1) * m.get(1, 1) + get(2, 2) * m.get(2, 1) + get(2, 3) * m.get(3, 1);
	n[2][2] = get(2, 0) * m.get(0, 2) + get(2, 1) * m.get(1, 2) + get(2, 2) * m.get(2, 2) + get(2, 3) * m.get(3, 2);
	n[2][3] = get(2, 0) * m.get(0, 3) + get(2, 1) * m.get(1, 3) + get(2, 2) * m.get(2, 3) + get(2, 3) * m.get(3, 3);
	n[3][0] = get(3, 0) * m.get(0, 0) + get(3, 1) * m.get(1, 0) + get(3, 2) * m.get(2, 0) + get(3, 3) * m.get(3, 0);
	n[3][1] = get(3, 0) * m.get(0, 1) + get(3, 1) * m.get(1, 1) + get(3, 2) * m.get(2, 1) + get(3, 3) * m.get(3, 1);
	n[3][2] = get(3, 0) * m.get(0, 2) + get(3, 1) * m.get(1, 2) + get(3, 2) * m.get(2, 2) + get(3, 3) * m.get(3, 2);
	n[3][3] = get(3, 0) * m.get(0, 3) + get(3, 1) * m.get(1, 3) + get(3, 2) * m.get(2, 3) + get(3, 3) * m.get(3, 3);

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			set(i, j, n[i][j]);
}


float Matrix44::determinant() const
{
	return
      get(0,3) * get(1,2) * get(2,1) * get(3,0)-get(0,2) * get(1,3) * get(2,1) * get(3,0)-get(0,3) * get(1,1) * get(2,2) * get(3,0)+get(0,1) * get(1,3) * get(2,2) * get(3,0) +
      get(0,2) * get(1,1) * get(2,3) * get(3,0)-get(0,1) * get(1,2) * get(2,3) * get(3,0)-get(0,3) * get(1,2) * get(2,0) * get(3,1)+get(0,2) * get(1,3) * get(2,0) * get(3,1) +
      get(0,3) * get(1,0) * get(2,2) * get(3,1)-get(0,0) * get(1,3) * get(2,2) * get(3,1)-get(0,2) * get(1,0) * get(2,3) * get(3,1)+get(0,0) * get(1,2) * get(2,3) * get(3,1) +
      get(0,3) * get(1,1) * get(2,0) * get(3,2)-get(0,1) * get(1,3) * get(2,0) * get(3,2)-get(0,3) * get(1,0) * get(2,1) * get(3,2)+get(0,0) * get(1,3) * get(2,1) * get(3,2) +
      get(0,1) * get(1,0) * get(2,3) * get(3,2)-get(0,0) * get(1,1) * get(2,3) * get(3,2)-get(0,2) * get(1,1) * get(2,0) * get(3,3)+get(0,1) * get(1,2) * get(2,0) * get(3,3) +
      get(0,2) * get(1,0) * get(2,1) * get(3,3)-get(0,0) * get(1,2) * get(2,1) * get(3,3)-get(0,1) * get(1,0) * get(2,2) * get(3,3)+get(0,0) * get(1,1) * get(2,2) * get(3,3);
}

void Matrix44::invert()
{
	// bugged ofc: need to make a new Matrix44!

	//set(0,0, get(1,2)*get(2,3)*get(3,1)-get(1,3)*get(2,2)*get(3,1)+get(1,3)*get(2,1)*get(3,2)-get(1,1)*get(2,3)*get(3,2)- get(1,2)*get(2,1)*get(3,3)+get(1,1)*get(2,2)*get(3,3));
	//set(0,1, get(0,3)*get(2,2)*get(3,1)-get(0,2)*get(2,3)*get(3,1)-get(0,3)*get(2,1)*get(3,2)+get(0,1)*get(2,3)*get(3,2)+ get(0,2)*get(2,1)*get(3,3)-get(0,1)*get(2,2)*get(3,3));
	//set(0,2, get(0,2)*get(1,3)*get(3,1)-get(0,3)*get(1,2)*get(3,1)+get(0,3)*get(1,1)*get(3,2)-get(0,1)*get(1,3)*get(3,2)- get(0,2)*get(1,1)*get(3,3)+get(0,1)*get(1,2)*get(3,3));
	//set(0,3, get(0,3)*get(1,2)*get(2,1)-get(0,2)*get(1,3)*get(2,1)-get(0,3)*get(1,1)*get(2,2)+get(0,1)*get(1,3)*get(2,2)+ get(0,2)*get(1,1)*get(2,3)-get(0,1)*get(1,2)*get(2,3));
	//set(1,0, get(1,3)*get(2,2)*get(3,0)-get(1,2)*get(2,3)*get(3,0)-get(1,3)*get(2,0)*get(3,2)+get(1,0)*get(2,3)*get(3,2)+ get(1,2)*get(2,0)*get(3,3)-get(1,0)*get(2,2)*get(3,3));
	//set(1,1, get(0,2)*get(2,3)*get(3,0)-get(0,3)*get(2,2)*get(3,0)+get(0,3)*get(2,0)*get(3,2)-get(0,0)*get(2,3)*get(3,2)- get(0,2)*get(2,0)*get(3,3)+get(0,0)*get(2,2)*get(3,3));
	//set(1,2, get(0,3)*get(1,2)*get(3,0)-get(0,2)*get(1,3)*get(3,0)-get(0,3)*get(1,0)*get(3,2)+get(0,0)*get(1,3)*get(3,2)+ get(0,2)*get(1,0)*get(3,3)-get(0,0)*get(1,2)*get(3,3));
	//set(1,3, get(0,2)*get(1,3)*get(2,0)-get(0,3)*get(1,2)*get(2,0)+get(0,3)*get(1,0)*get(2,2)-get(0,0)*get(1,3)*get(2,2)- get(0,2)*get(1,0)*get(2,3)+get(0,0)*get(1,2)*get(2,3));
	//set(2,0, get(1,1)*get(2,3)*get(3,0)-get(1,3)*get(2,1)*get(3,0)+get(1,3)*get(2,0)*get(3,1)-get(1,0)*get(2,3)*get(3,1)- get(1,1)*get(2,0)*get(3,3)+get(1,0)*get(2,1)*get(3,3));
	//set(2,1, get(0,3)*get(2,1)*get(3,0)-get(0,1)*get(2,3)*get(3,0)-get(0,3)*get(2,0)*get(3,1)+get(0,0)*get(2,3)*get(3,1)+ get(0,1)*get(2,0)*get(3,3)-get(0,0)*get(2,1)*get(3,3));
	//set(2,2, get(0,1)*get(1,3)*get(3,0)-get(0,3)*get(1,1)*get(3,0)+get(0,3)*get(1,0)*get(3,1)-get(0,0)*get(1,3)*get(3,1)- get(0,1)*get(1,0)*get(3,3)+get(0,0)*get(1,1)*get(3,3));
	//set(2,3, get(0,3)*get(1,1)*get(2,0)-get(0,1)*get(1,3)*get(2,0)-get(0,3)*get(1,0)*get(2,1)+get(0,0)*get(1,3)*get(2,1)+ get(0,1)*get(1,0)*get(2,3)-get(0,0)*get(1,1)*get(2,3));
	//set(3,0, get(1,2)*get(2,1)*get(3,0)-get(1,1)*get(2,2)*get(3,0)-get(1,2)*get(2,0)*get(3,1)+get(1,0)*get(2,2)*get(3,1)+ get(1,1)*get(2,0)*get(3,2)-get(1,0)*get(2,1)*get(3,2));
	//set(3,1, get(0,1)*get(2,2)*get(3,0)-get(0,2)*get(2,1)*get(3,0)+get(0,2)*get(2,0)*get(3,1)-get(0,0)*get(2,2)*get(3,1)- get(0,1)*get(2,0)*get(3,2)+get(0,0)*get(2,1)*get(3,2));
	//set(3,2, get(0,2)*get(1,1)*get(3,0)-get(0,1)*get(1,2)*get(3,0)-get(0,2)*get(1,0)*get(3,1)+get(0,0)*get(1,2)*get(3,1)+ get(0,1)*get(1,0)*get(3,2)-get(0,0)*get(1,1)*get(3,2));
	//set(3,3, get(0,1)*get(1,2)*get(2,0)-get(0,2)*get(1,1)*get(2,0)+get(0,2)*get(1,0)*get(2,1)-get(0,0)*get(1,2)*get(2,1)- get(0,1)*get(1,0)*get(2,2)+get(0,0)*get(1,1)*get(2,2));
	//multiply(1 / determinant());

	float invMat[16];

	invMat[0] = m_components[0];  invMat[4] = m_components[1];  invMat[8] = m_components[2];           invMat[12] = -m_components[0]*m_components[12] - m_components[1]*m_components[13] - m_components[2]*m_components[14];
	invMat[1] = m_components[4];  invMat[5] = m_components[5];  invMat[9] = m_components[6];           invMat[13] = -m_components[4]*m_components[12] - m_components[5]*m_components[13] - m_components[6]*m_components[14];
	invMat[2] = m_components[8];  invMat[6] = m_components[9];  invMat[10] = m_components[10];         invMat[14] = -m_components[8]*m_components[12] - m_components[9]*m_components[13] - m_components[10]*m_components[14];
	invMat[3] = 0;       invMat[7] = 0;       invMat[11] = 0;               invMat[15] = 1.0f;

	for(int i = 0; i < 16; i++) 
		m_components[i] = invMat[i];
}