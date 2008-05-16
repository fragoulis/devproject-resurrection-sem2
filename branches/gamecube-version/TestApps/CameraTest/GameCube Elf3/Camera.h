#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <demo.h> //TODO: REMOVE AND INCLUDE THE CORRECT HEADER FOR THE FRAMEWORK

#include <math.h>
#include "Vector3.h"


#define PI 3.1415926535897932384626433832795

/*
	Camera class
*/


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


	//TODO: REMOVE, GET THE VIEW MATRIX FROM THE RENDERING ENGINE
	static Mtx viewMatrix;
};



#endif