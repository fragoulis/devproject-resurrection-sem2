//*****************************************************************************
//**
//**   File:               Camera.cpp
//**   Author:             $Author: esrever2357 $
//**   Last Change:        $Date: 2008-03-19 00:00:25 +0000 (Wed, 19 Mar 2008) $
//**   Last Revision:      $Revision: 186 $
//**
//*****************************************************************************

#include "Camera.h"

Camera::Camera()
{

}

void Camera::setPerspective(float vAng, float asp, float nearD, float farD)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(vAng, asp, nearD, farD);
	//to avoid to use glu a conversion from gluPerspective to glFrustum is done
	fH = float(tan( vAng / 360 * PI ) * nearD);
	fW = fH * asp;
	fNear = nearD;
	fFar = farD;

	glFrustum( -fW, fW, -fH, fH, nearD, farD );
}

void Camera::setModelViewMatrix()
{
	float m[16];
	Vector3 eVec(eye.getX(), eye.getY(), eye.getZ());
	m[0] = u.getX(); m[4] = u.getY(); m[8] = u.getZ();  m[12] = -eVec.dot(u);
	m[1] = v.getX(); m[5] = v.getY(); m[9] = v.getZ();  m[13] = -eVec.dot(v);
	m[2] = n.getX(); m[6] = n.getY(); m[10] = n.getZ(); m[14] = -eVec.dot(n);
	m[3] = 0;   m[7] = 0;   m[11] = 0;   m[15] = 1.0;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(m);
}

void Camera::setPosition(Vector3 Eye, Vector3 look, Vector3 up)
{
	eye = Eye;
	n = Vector3(Eye.getX() - look.getX(), Eye.getY() - look.getY(), Eye.getZ() - look.getZ());
	u = Vector3::cross(up, n);
	n.normalize(); 
	u.normalize();
	v = Vector3::cross(n, u);
	setModelViewMatrix();
}

void Camera::slide(float delU, float delV, float delN)
{
	eye.setX(eye.getX() + delU*u.getX() + delV*v.getX() + delN*n.getX());
	eye.setY(eye.getY() + delU*u.getY() + delV*v.getY() + delN*n.getY());
	eye.setZ(eye.getZ() + delU*u.getZ() + delV*v.getZ() + delN*n.getZ());
	setModelViewMatrix();
}


void Camera::roll(float angle)
{
	float cs = (float) cos(PI/180 * angle);
	float sn = (float) sin(PI/180 * angle);
	Vector3 t = u;
	u.set(cs*t.getX() - sn*v.getX(), cs*t.getY() - sn*v.getY(), cs*t.getZ() - sn*v.getZ());
	v.set(sn*t.getX() + cs*v.getX(), sn*t.getY() + cs*v.getY(), sn*t.getZ() + cs*v.getZ());
	setModelViewMatrix();
}

void Camera::pitch(float angle) 
{
	float cs = (float) cos(PI/180 * angle);
	float sn = (float) sin(PI/180 * angle);
	Vector3 t = n;
	n.set(cs*t.getX() - sn*v.getX(), cs*t.getY() - sn*v.getY(), cs*t.getZ() - sn*v.getZ());
	v.set(sn*t.getX() + cs*v.getX(), sn*t.getY() + cs*v.getY(), sn*t.getZ() + cs*v.getZ());
	setModelViewMatrix();
}

void Camera::yaw(float angle) 
{
	float cs = (float) cos(PI/180 * angle);
	float sn = (float) sin(PI/180 * angle);
	Vector3 t = n;
	n.set(cs*t.getX() - sn*u.getX(), cs*t.getY() - sn*u.getY(), cs*t.getZ() - sn*u.getZ());
	u.set(sn*t.getX() + cs*u.getX(), sn*t.getY() + cs*u.getY(), sn*t.getZ() + cs*u.getZ());
	setModelViewMatrix();
}

void Camera :: getProjSettings(float & w,float & h, float & n, float &f) const
{
	w = fW;
	h = fH;
	n = fNear;
	f = fFar;
}