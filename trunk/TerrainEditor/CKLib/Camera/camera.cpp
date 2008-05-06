#include <Camera/camera.h>
#include <Math/quat.h>
#include <Misc/utils.h>
#include <Math/math_std.h>

using namespace CKLib;
using namespace std;

vec3<float>	Camera :: _rotSpeed(1.0f);
vec3<float> Camera :: _moveSpeed(1.0f);

Camera :: Camera(const vec3<float>& pos,
				 const vec3<float>& view,
				 const vec3<float>& up)
:_pos(pos),
_view(view),
_up(up)
{
	_proj_params[0] = _proj_params[2] = -1.0f;
	_proj_params[1] = _proj_params[3] = _proj_params[5] = 1.0f;
	_proj_params[4] = 0.1f;

	_right = _view.cross(_up);
}

Camera :: ~Camera()
{
}

void Camera ::getProjParams(float *params) const
{
	params[0] = _proj_params[0];
	params[1] = _proj_params[1];
	params[2] = _proj_params[2];
	params[3] = _proj_params[3];
	params[4] = _proj_params[4];
	params[5] = _proj_params[5];
}

const float * const Camera ::getProjParams() const
{
	return _proj_params;
}

void Camera ::setFrustum(const float& left,
					const float& right,
					const float& bottom,
					const float& top,
					const float& anear,
					const float& afar)
{
	_proj_params[0] = left;
	_proj_params[1] = right;
	_proj_params[2] = bottom;
	_proj_params[3] = top;
	_proj_params[4] = anear;
	_proj_params[5] = afar;
	_aspect = right / top;
	setFrustum();
}


void Camera ::setOrtho(const float& left,
				  const float& right,
				  const float& bottom,
				  const float& top,
				  const float& anear,
				  const float& afar)
{
	_proj_params[0] = left;
	_proj_params[1] = right;
	_proj_params[2] = bottom;
	_proj_params[3] = top;
	_proj_params[4] = anear;
	_proj_params[5] = afar;
	_aspect = right / top;
	setOrtho();
}

void Camera :: computeFrustum()
{
	float modelview[16],projection[16];
	glGetFloatv(GL_PROJECTION_MATRIX,projection);
	glGetFloatv(GL_MODELVIEW_MATRIX,modelview);
	glPushMatrix();
	glLoadMatrixf(projection);
	glMultMatrixf(modelview);
	glGetFloatv(GL_MODELVIEW_MATRIX,modelview);
	glPopMatrix();

	float mgn;

	// Left clipping plane
	frustum_planes[0][0] = modelview[3] + modelview[0];
	frustum_planes[0][1] = modelview[7] + modelview[4];
	frustum_planes[0][2] = modelview[11] + modelview[8];
	frustum_planes[0][3] = modelview[15] + modelview[12];
	mgn = 1.0f/sqrtf(frustum_planes[0][0]*frustum_planes[0][0] + 
			frustum_planes[0][1]*frustum_planes[0][1] + 
			frustum_planes[0][2]*frustum_planes[0][2]);
	frustum_planes[0][0] *= mgn;
	frustum_planes[0][1] *= mgn;
	frustum_planes[0][2] *= mgn;
	frustum_planes[0][3] *= mgn;

	// Right clipping plane
	frustum_planes[1][0] = modelview[3] - modelview[0];
	frustum_planes[1][1] = modelview[7] - modelview[4];
	frustum_planes[1][2] = modelview[11] - modelview[8];
	frustum_planes[1][3] = modelview[15] - modelview[12];
	mgn = 1.0f/sqrtf(frustum_planes[1][0]*frustum_planes[1][0] + 
			frustum_planes[1][1]*frustum_planes[1][1] + 
			frustum_planes[1][2]*frustum_planes[1][2]);
	frustum_planes[1][0] *= mgn;
	frustum_planes[1][1] *= mgn;
	frustum_planes[1][2] *= mgn;
	frustum_planes[1][3] *= mgn;

	// Top clipping plane
	frustum_planes[2][0] = modelview[3] - modelview[1];
	frustum_planes[2][1] = modelview[7] - modelview[5];
	frustum_planes[2][2] = modelview[11] - modelview[9];
	frustum_planes[2][3] = modelview[15] - modelview[13];
	mgn = 1.0f/sqrtf(frustum_planes[2][0]*frustum_planes[2][0] + 
			frustum_planes[2][1]*frustum_planes[2][1] + 
			frustum_planes[2][2]*frustum_planes[2][2]);
	frustum_planes[2][0] *= mgn;
	frustum_planes[2][1] *= mgn;
	frustum_planes[2][2] *= mgn;
	frustum_planes[2][3] *= mgn;

	// Bottom clipping plane
	frustum_planes[3][0] = modelview[3] + modelview[1];
	frustum_planes[3][1] = modelview[7] + modelview[5];
	frustum_planes[3][2] = modelview[11] + modelview[9];
	frustum_planes[3][3] = modelview[15] + modelview[13];
	mgn = 1.0f/sqrtf(frustum_planes[3][0]*frustum_planes[3][0] + 
			frustum_planes[3][1]*frustum_planes[3][1] + 
			frustum_planes[3][2]*frustum_planes[3][2]);
	frustum_planes[3][0] *= mgn;
	frustum_planes[3][1] *= mgn;
	frustum_planes[3][2] *= mgn;
	frustum_planes[3][3] *= mgn;

	// Near clipping plane
	frustum_planes[4][0] = modelview[3] + modelview[2];
	frustum_planes[4][1] = modelview[7] + modelview[6];
	frustum_planes[4][2] = modelview[11] + modelview[10];
	frustum_planes[4][3] = modelview[15] + modelview[14];
	mgn = 1.0f/sqrtf(frustum_planes[4][0]*frustum_planes[4][0] + 
			frustum_planes[4][1]*frustum_planes[4][1] + 
			frustum_planes[4][2]*frustum_planes[4][2]);
	frustum_planes[4][0] *= mgn;
	frustum_planes[4][1] *= mgn;
	frustum_planes[4][2] *= mgn;
	frustum_planes[4][3] *= mgn;

	// Far clipping plane
	frustum_planes[5][0] = modelview[3] - modelview[2];
	frustum_planes[5][1] = modelview[7] - modelview[6];
	frustum_planes[5][2] = modelview[11] - modelview[10];
	frustum_planes[5][3] = modelview[15] - modelview[14];
	mgn = 1.0f/sqrtf(frustum_planes[5][0]*frustum_planes[5][0] + 
			frustum_planes[5][1]*frustum_planes[5][1] + 
			frustum_planes[5][2]*frustum_planes[5][2]);
	frustum_planes[5][0] *= mgn;
	frustum_planes[5][1] *= mgn;
	frustum_planes[5][2] *= mgn;
	frustum_planes[5][3] *= mgn;
}


// In the below rotations, the 2 rots can be 1 rot & one cross
// Check which is better
void Camera ::pitch(const float& amount)
{
	// rotate up & view vector around right vector
	quat<float> q(amount*_rotSpeed(0),_right);
	q.normalize();
	q.transform(_up);
	q.transform(_view);
}

void Camera ::yaw(const float& amount)
{
	// rotate right & view vector around up vector
	quat<float> q(amount*_rotSpeed(1),_up);
	q.normalize();
	q.transform(_right);
	q.transform(_view);			//  **** THIS IS FAST!!
	//_view = _up.cross(_right);
}
void Camera ::roll(const float& amount)
{
	// rotate up & right vector around view vector
	quat<float> q(amount*_rotSpeed(2),_view);
	q.normalize();
	q.transform(_right);
	q.transform(_up);
}
void Camera ::strafe(const float& amount)
{
	_pos += _right*amount*_moveSpeed(0);
}
void Camera ::move(const float& amount)
{
	_pos += _view*amount*_moveSpeed(2);
}

void Camera ::raise(const float& amount)
{
	_pos += _up*amount*_moveSpeed(1);
}

void Camera :: jitter(const float amount)
{
	_pos += vec3<float>((math_const<float>::rand_norm()-0.5f)*2.0f,
						(math_const<float>::rand_norm()-0.5f)*2.0f,
						(math_const<float>::rand_norm()-0.5f)*2.0f)*amount;
}

