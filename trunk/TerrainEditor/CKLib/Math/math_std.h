// Contains standard used vectors, matrices, quaternions etc
#ifndef _math_sth_h_
#define _math_sth_h_

#include <Math/vec3.h>
#include <Math/vec4.h>
#include <Math/matrix4.h>
#include <Math/matrix3.h>
#include <Math/quat.h>

namespace CKLib {

// Some useful constants

const vec3<float> X_AXIS3f = vec3<float>(1.0f,0.0f,0.0f);
const vec3<float> Y_AXIS3f = vec3<float>(0.0f,1.0f,0.0f);
const vec3<float> Z_AXIS3f = vec3<float>(0.0f,0.0f,1.0f);
const vec4<float> X_AXIS4f = vec4<float>(1.0f,0.0f,0.0f,1.0f);
const vec4<float> Y_AXIS4f = vec4<float>(0.0f,1.0f,0.0f,1.0f);
const vec4<float> Z_AXIS4f = vec4<float>(0.0f,0.0f,1.0f,1.0f);
const vec3<double> X_AXIS3d = vec3<double>(1.0f,0.0f,0.0f);
const vec3<double> Y_AXIS3d = vec3<double>(0.0f,1.0f,0.0f);
const vec3<double> Z_AXIS3d = vec3<double>(0.0f,0.0f,1.0f);
const vec4<double> X_AXIS4d = vec4<double>(1.0f,0.0f,0.0f,1.0f);
const vec4<double> Y_AXIS4d = vec4<double>(0.0f,1.0f,0.0f,1.0f);
const vec4<double> Z_AXIS4d = vec4<double>(0.0f,0.0f,1.0f,1.0f);
const vec3<float> I_AXIS3f[3] = {X_AXIS3f,Y_AXIS3f,Z_AXIS3f};
const vec4<float> I_AXIS4f[3] = {X_AXIS4f,Y_AXIS4f,Z_AXIS4f};
const vec3<float> ZERO_VECTOR3f = vec3<float>(0.0f,0.0f,0.0f);
const vec4<float> ZERO_VECTOR4f = vec4<float>(0.0f,0.0f,0.0f,0.0f);
const matrix44<float> IDENTITY44f = matrix44<float>(1.0f,0.0f,0.0f,0.0f,
													0.0f,1.0f,0.0f,0.0f,
													0.0f,0.0f,1.0f,0.0f,
													0.0f,0.0f,0.0f,1.0f);
const matrix44<double> IDENTITY44d = matrix44<double>(1.0f,0.0f,0.0f,0.0f,
													0.0f,1.0f,0.0f,0.0f,
													0.0f,0.0f,1.0f,0.0f,
													0.0f,0.0f,0.0f,1.0f);

// Quaternion to matrix conversion function
template <class T> matrix44<T> quat_to_matrix44(const quat<T>& q)
{
	         return matrix44<T>(1.0f - 2.0f * (q.y() * q.y() + q.z() * q.z()) ,  
							  2.0f * (q.x() * q.y() + q.z() * q.w()),
							  2.0f * (q.x() * q.z() - q.y() * q.w()),
							  0.0f,
							  2.0f * (q.x() * q.y() - q.z() * q.w()),
							  1.0f - 2.0f * (q.x() * q.x() + q.z() * q.z()),
							  2.0f * (q.z() * q.y() + q.x() * q.w()),
							  0.0f,
							  2.0f * (q.x() * q.z() + q.y() * q.w()),
							  2.0f * (q.y() * q.z() - q.x() * q.w()),
							  1.0f - 2.0f * (q.x() * q.x() + q.y() * q.y()),
							  0.0f,
							  0.0f,
							  0.0f,
							  0.0f,
							  1.0f);
}

// Quaternion to matrix conversion function
template <class T> matrix33<T> quat_to_matrix33(const quat<T>& q)
{
	         return matrix33<T>(1.0f - 2.0f * (q.y() * q.y() + q.z() * q.z()) ,  
							  2.0f * (q.x() * q.y() + q.z() * q.w()),
							  2.0f * (q.x() * q.z() - q.y() * q.w()),
							  2.0f * (q.x() * q.y() - q.z() * q.w()),
							  1.0f - 2.0f * (q.x() * q.x() + q.z() * q.z()),
							  2.0f * (q.z() * q.y() + q.x() * q.w()),
							  2.0f * (q.x() * q.z() + q.y() * q.w()),
							  2.0f * (q.y() * q.z() - q.x() * q.w()),
							  1.0f - 2.0f * (q.x() * q.x() + q.y() * q.y()));
}

// Matrix to quaternion to  conversion function
template <class T> quat<T> matrix33_to_quat(const matrix33<T>& mat)
{
	float trace = mat(0,0) + mat(1,1) + mat(2,2) + 1.0f;
	quat<T> q;
	if( trace > THRESHOLD<float>() ) 
	{
		float s = 0.5f / sqrtf(trace);
		q.w() = 0.25f / s;
		q.x() = ( mat(1,2) - mat(2,1) ) * s;
		q.y() = ( mat(2,0) - mat(0,2) ) * s;
		q.z() = ( mat(0,1) - mat(1,0) ) * s;
	 } 
	else 
	{
		if ( mat(0,0) > mat(1,1) && mat(0,0) > mat(2,2) ) 
		{
		  float s = 2.0f * sqrtf( 1.0f + mat(0,0) - mat(1,1) - mat(2,2));
		  q.w() = (mat(2,1) - mat(1,2) ) / s;
		  q.x() = 0.25f * s;
		  q.y() = (mat(1,0) + mat(0,1) ) / s;
		  q.z() = (mat(2,0) + mat(0,2) ) / s;
		} 
		else if (mat(1,1) > mat(2,2)) 
		{
		  float s = 2.0f * sqrtf( 1.0f + mat(1,1) - mat(0,0) - mat(2,2));
		  q.w() = (mat(2,0) - mat(0,2) ) / s;
		  q.x() = (mat(1,0) + mat(0,1) ) / s;
		  q.y() = 0.25f * s;
		  q.z() = (mat(2,1) + mat(1,2) ) / s;
		} 
		else 
		{
		  float s = 2.0f * sqrtf( 1.0f + mat(2,2) - mat(0,0) - mat(1,1) );
		  q.w() = (mat(1,0) - mat(0,1) ) / s;
		  q.x() = (mat(2,0) + mat(0,2) ) / s;
		  q.y() = (mat(2,1) + mat(1,2) ) / s;
		  q.z() = 0.25f * s;
		}
	}
	return q;
}



}

#endif