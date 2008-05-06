template <class T>
quat<T> :: quat (const T& angle,const vec3<T>& axis)
{
	// Convert the angle degrees into radians.
	const T half_angle_rad = angle * T(math_const<T>::DEG2RAD * 0.5);

    // Call this once for optimization, just like using the if statement to determine if
    // we should normalize.
	const T sine = sin(half_angle_rad);
 
	// Create the quaternion.
	x() = axis(0) * sine;
	y() = axis(1) * sine;
	z() = axis(2) * sine;
    w() = cos(half_angle_rad);

	//normalize();
}

template <class T>
void quat<T> :: setRotation(const T& angle,const vec3<T>& axis)
{
	// Convert the angle degrees into radians.
	const T half_angle_rad = angle * T(math_const<T>::DEG2RAD * 0.5);

    // Call this once for optimization, just like using the if statement to determine if
    // we should normalize.
	const T sine = sin(half_angle_rad);
 
	// Create the quaternion.
	x() = axis(0) * sine;
	y() = axis(1) * sine;
	z() = axis(2) * sine;
    w() = cos(half_angle_rad);

	normalize();
}

template <class T>
void quat<T> :: rotateBy(const quat& q)
{
	const float tmp[8] = { 
		(q.w() + q.x())*(w() + x()),
		(q.z() - q.y())*(y() - z()),
		(q.w() - q.x())*(y() + z()),
		(q.y() + q.z())*(w() - x()),
		(q.x() + q.z())*(x() + y()),
		(q.x() - q.z())*(x() - y()),
		(q.w() + q.y())*(w() - z()),
		(q.w() - q.y())*(w() + z())
	};
	_vec[0] = tmp[0] - (tmp[4] + tmp[5] + tmp[6] + tmp[7])*0.5f;
	_vec[1] = tmp[2] + (tmp[4] - tmp[5] + tmp[6] - tmp[7])*0.5f;
	_vec[2] = tmp[3] + (tmp[4] - tmp[5] - tmp[6] + tmp[7])*0.5f;
	_vec[3] = tmp[1] + (-tmp[4] - tmp[5] + tmp[6] + tmp[7])*0.5f;
}

template <class T>
void quat<T> :: transform(vec3<T>& v) const
{
	const quat qtmp(v(0),v(1),v(2),T(0));
	v = represent<quat<float>,vec3<float> >(((*this) * qtmp) * ~(*this));	// *** THIS IS FAST!!
	return;
}

template <class T>
vec3<T> quat<T> :: transform(const vec3<T>& v) const
{
	const quat qtmp(v(0),v(1),v(2),T(0));
	return represent<quat<float>,vec3<float> >(((*this) * qtmp) * ~(*this));	// *** THIS IS FAST!!
}

// STATIC

template <class T>
quat<T> quat<T> :: slerp(const quat<T>& qsrc,const quat<T>& qdst,const float t)
{
    float   to1[4];
    float  omega, cosom, sinom_denom, scale0, scale1;

    // calc cosine
    cosom = qsrc.x() * qdst.x() + qsrc.y() * qdst.y() + qsrc.z() * qdst.z()
		       + qsrc.w() * qdst.w();

    // adjust signs (if necessary)
    if ( cosom <0.0f )
	{ 
		cosom = -cosom; to1[0] = - qdst.x();
		to1[1] = - qdst.y();
		to1[2] = - qdst.z();
		to1[3] = - qdst.w();
    } 
	else  
	{
		to1[0] = qdst.x();
		to1[1] = qdst.y();
		to1[2] = qdst.z();
		to1[3] = qdst.w();
    }
    // calculate coefficients


    if ( (1.0f - cosom) > THRESHOLD<float>() ) 
	{
        // standard case (slerp)
        omega = acosf(cosom);
        sinom_denom = 1.0f / sinf(omega);
        scale0 = sinf((1.0f - t) * omega) * sinom_denom;
        scale1 = sinf(t * omega) * sinom_denom;
    } 
	else 
	{        
	// "from" and "to" quaternions are very close 
    //  ... so we can do a linear interpolation
            scale0 = 1.0f - t;
            scale1 = t;
    }
	// calculate final values
	return quat<T>(scale0 * qsrc.x() + scale1 * to1[0],
				   scale0 * qsrc.y() + scale1 * to1[1],
				   scale0 * qsrc.z() + scale1 * to1[2],
				   scale0 * qsrc.w() + scale1 * to1[3]);
}

/*
	NON_MEMBER FUNCTIONS
*/

// Streaming Operators
template <class T>
ostream& operator <<(ostream& os,const quat<T>& v)
{
	os<<v.x()<<", "<<v.y()<<", "<<v.z()<<", "<<v.w();
	return os;
}

template <class T>
istream& operator >>(istream& is,quat<T>& v)
{
	is>>v.x()>>v.y()>>v.z()>>v.w();
	return is;
}
