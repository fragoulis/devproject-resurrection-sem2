// information access

#include <windows.h>
#include <glee.h>
#include <GL/glu.h>

inline const vec3<float>& Camera :: pos() const
{return _pos;}
inline vec3<float>& Camera :: pos()
{return _pos;}
inline const vec3<float>& Camera :: view() const
{return _view;}
inline vec3<float>& Camera :: view()
{return _view;}
inline const vec3<float>& Camera :: up() const
{return _up;}
inline vec3<float>& Camera :: up()
{return _up;}

inline float Camera :: aspect() const 
{return _aspect;}

inline void  Camera :: getFrustum(float frplanes[6][4]) const
{memcpy(frplanes,frustum_planes,24*sizeof(float));}

inline void Camera ::setEyeSpace() const
{
	gluLookAt(_pos(0),_pos(1),_pos(2),
			  _pos(0)+_view(0),_pos(1)+_view(1),_pos(2)+_view(2),
			  _up(0),_up(1),_up(2));
}

inline void Camera :: setFrustum() const
{
	glPushAttrib(GL_MATRIX_MODE);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(_proj_params[0],_proj_params[1],_proj_params[2],_proj_params[3],_proj_params[4],_proj_params[5]);
	glPopAttrib();
}

inline void Camera :: setOrtho() const
{
	glPushAttrib(GL_MATRIX_MODE);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(_proj_params[0],_proj_params[1],_proj_params[2],_proj_params[3],_proj_params[4],_proj_params[5]);
	glPopAttrib();
}
