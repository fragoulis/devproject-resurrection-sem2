#ifndef _camera_h_
#define _camera_h_

#include <Math/vec3.h>

namespace CKLib {

/*
	Camera class :
	Uses vectors for pos, up,right & view
	Keeps frustum parameters
*/
class Camera
{
protected :
	vec3<float>		_pos,_view,_up,_right;
	
	float			_proj_params[6];
	float			frustum_planes[6][4];

	float			_aspect;

	static vec3<float>	_rotSpeed;
	static vec3<float>  _moveSpeed;

public : 
	// ctors
	Camera(const vec3<float>& pos = vec3<float>(0.0f,0.0f,1.0f),
		   const vec3<float>& view = vec3<float>(0.0f,0.0f,-1.0f),
		   const vec3<float>& up = vec3<float>(0.0f,1.0f,0.0f));

	// dtor
	~Camera();

	// setting information in GL
	void setFrustum(const float& left,
					const float& right,
					const float& bottom,
					const float& top,
					const float& anear,
					const float& afar);
	void setFrustum() const;
	void setOrtho(const float& left,
				  const float& right,
				  const float& bottom,
				  const float& top,
				  const float& anear,
				  const float& afar);
	void setOrtho() const;
	void setEyeSpace() const;			// Sets the GL matrix for the camera
	void computeFrustum();				
	void getFrustum(float frplanes[6][4]) const;	
	void jitter(const float amount);	// Jitters the position of the camera based on a given amount

	// information access
	void getProjParams(float *params) const;
	const float * const getProjParams() const;
	const vec3<float>& pos() const;
	vec3<float>& pos();
	const vec3<float>& view() const;
	vec3<float>& view();
	const vec3<float>& up() const;
	vec3<float>& up();
	float aspect() const;		// the aspect ratio of the projection

	// utility funcs
	void pitch(const float& amount);
	void yaw(const float& amount);
	void roll(const float& amount);
	void strafe(const float& amount);
	void move(const float& amount);
	void raise(const float& amount);

	// Static 
	//static void parseConfiguration(const Parser& parser,const unsigned i);


};

#include <Camera/Camera.inl>

}
#endif