//*****************************************************************************
//**
//**   File:               Camera.h
//**   Author:             $Author: esrever2357 $
//**   Last Change:        $Date: 2008-03-19 00:00:25 +0000 (Wed, 19 Mar 2008) $
//**   Last Revision:      $Revision: 186 $
//**
//*****************************************************************************

#include <math.h>
#include "../Math/Vector3.h"
#include <gl/glee.h>
#include <gl/gl.h>
#include <gl/glu.h>


const GLdouble PI = 3.1415926535897932384626433832795;


class Camera 
{
private:
	Vector3 eye;
	Vector3 u,v,n;
	double viewAngle, aspect;
	void setModelViewMatrix();

	float fW,fH,fNear,fFar;

public:
	Camera();
	virtual ~Camera(){}
	void setPosition(Vector3 eye, Vector3 look, Vector3 up);
	void roll(float angle);
	void pitch(float angle);
	void yaw(float angle);
	void slide(float delU, float delV, float delN);
	void setPerspective(float vAng, float asp, float nearD, float farD);
	virtual void update(const float dt) {}

	const Vector3 &getEye() const {return eye;};
	const Vector3 &getLookAt() const {return n;};
	void getProjSettings(float & w,float & h, float & n, float &f) const;
};