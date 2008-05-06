#ifndef _quat_h_
#define _quat_h_

#include <Misc/defines.h>
#include <Math/matrix4.h>

namespace CKLib {

/*
	Quaternion Class
*/
template <class T>
class quat 
{
	T	_vec[4];
public : 
	// ctor
	quat (const T& angle,const vec3<T>& axis);
	quat (const T& x,const T& y,const T& z,const T& w);
	quat ();

	// operators
	const quat operator *(const T& s) const;
	const quat operator *(const quat& q) const;
	const quat operator +(const quat& q) const;
	const quat operator ~() const; // conjugate
	const quat& operator *=(const T& s);
	const quat& operator *=(const quat& q);

	// access
	const T& x() const;
	T& x();
	const T& y() const;
	T& y();
	const T& z() const;
	T& z();
	const T& w() const;
	T& w();

	// utilities
	void setRotation(const T& angle,const vec3<T>& axis);
	void transform(vec3<T>& v) const;
	vec3<T> transform(const vec3<T>& v) const;
	void normalize();
	const T	 length() const;
	const T	 sqLength() const;
	const matrix44<T> toMatrix() const;
	void rotateBy(const quat& q);

	static quat<T> slerp(const quat<T>& qsrc,const quat<T>& qdst,const float t);
	static quat<T> nlerp(const quat<T>& qsrc,const quat<T>& qdst,const float t);
	//return a matrix ?? or in non-member function?? (math_std)
};

#include <Math/quat.inl>
#include <Math/quat.hpp>

}
#endif