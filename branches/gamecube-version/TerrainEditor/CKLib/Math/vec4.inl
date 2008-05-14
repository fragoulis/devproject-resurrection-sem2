// ctors

template <class T> inline vec4<T> :: vec4(const T& scalar)
{_v[0] = _v[1] = _v[2] = _v[3] = scalar;}

template <class T> inline vec4<T> :: vec4(const T& scalar1,const T& scalar2,const T& scalar3,const T& scalar4)
{_v[0] = scalar1;_v[1] = scalar2;_v[2] = scalar3;_v[3] = scalar4;}

template <class T> inline vec4<T> :: vec4(const vec4<T>& v)
{_v[0] = v(0);_v[1] = v(1);_v[2] = v(2);_v[3] = v(3);}

// access
template <class T> inline const T& vec4<T> :: operator ()(const unsigned i) const
{ASSERT(i < 4);return _v[i];}
template <class T> inline T& vec4<T> :: operator()(const unsigned i)
{ASSERT(i < 4);return _v[i];}
template <class T> inline const T * const vec4<T> :: get() const
{return static_cast<const T *>(_v);}
template <class T> inline const T& vec4<T> :: x() const {return _v[0];}
template <class T> inline T& vec4<T> :: x() {return _v[0];}
template <class T> inline const T& vec4<T> :: y() const {return _v[1];}
template <class T> inline T& vec4<T> :: y() {return _v[1];}
template <class T> inline const T& vec4<T> :: z() const {return _v[2];}
template <class T> inline T& vec4<T> :: z() {return _v[2];}
template <class T> inline const T& vec4<T> :: w() const {return _v[3];}
template <class T> inline T& vec4<T> :: w() {return _v[3];}

// arithmetic vec4 - vec4
template <class T> inline const vec4<T> vec4<T> :: operator +(const vec4& v) const
{return vec4(_v[0] + v(0),_v[1] + v(1),_v[2] + v(2),_v[3] + v(3));}

template <class T> inline const vec4<T> vec4<T> :: operator -(const vec4& v) const
{return vec4(_v[0] - v(0),_v[1] - v(1),_v[2] - v(2),_v[3] - v(3));}

template <class T> inline const vec4<T> vec4<T> :: operator /(const vec4& v) const
{return vec4(_v[0]/v(0),_v[1]/v(1),_v[2]/v(2),_v[3]/v(3));}

template <class T> inline const vec4<T> vec4<T> :: operator *(const vec4& v) const
{return vec4(_v[0]*v(0),_v[1]*v(1),_v[2]*v(2),_v[3]*v(3));}

template <class T> inline const vec4<T> vec4<T> :: operator ^(const vec4& v) const
{return vec4(POW(_v[0],v(0)),POW(_v[1],v(1)),POW(_v[2],v(2)),POW(_v[3],v(3)));}

template <class T> inline const vec4<T>& vec4<T> :: operator +=(const vec4& v)
{
	_v[0] += v(0);
	_v[1] += v(1);
	_v[2] += v(2);
	_v[3] += v(3);
	return *this;
}

template <class T> inline const vec4<T>& vec4<T> :: operator -=(const vec4& v)
{
	_v[0] -= v(0);
	_v[1] -= v(1);
	_v[2] -= v(2);
	_v[3] -= v(3);
	return *this;
}

template <class T> inline const vec4<T>& vec4<T> :: operator /=(const vec4& v)
{
	_v[0] /= v(0);
	_v[1] /= v(1);
	_v[2] /= v(2);
	_v[3] /= v(3);
	return *this;
}

template <class T> inline const vec4<T>& vec4<T> :: operator *=(const vec4& v)
{
	_v[0] *= v(0);
	_v[1] *= v(1);
	_v[2] *= v(2);
	_v[3] *= v(3);
	return *this;
}

template <class T> inline const vec4<T>& vec4<T> :: operator ^=(const vec4& v)
{
	_v[0] = POW(_v[0],v(0));
	_v[1] = POW(_v[1],v(1));
	_v[2] = POW(_v[2],v(2));
	_v[3] = POW(_v[3],v(3));
	return *this;
}

template <class T> inline const vec4<T>& vec4<T> :: operator =(const vec4& v)
{
	_v[0] = v(0);
	_v[1] = v(1);
	_v[2] = v(2);
	_v[3] = v(3);
	// FIXME : (SIMD code) simdval = _mm_load_ps(v.get());
	return *this;
}

// arithmetic vec4 - scalar

template <class T> inline const vec4<T> vec4<T> :: operator +(const T& s) const
{return vec4(_v[0] + s,_v[1] + s,_v[2] + s,_v[3] + s);}

template <class T> inline const vec4<T> vec4<T> :: operator -(const T& s) const
{return vec4(_v[0] - s,_v[1] - s,_v[2] - s,_v[3] - s);}

template <class T>
inline const vec4<T> vec4<T> :: operator /(const T& s) const
{
	ASSERT(s != T(0));
	const float tmp = 1 / s;
	return vec4(_v[0] *tmp,_v[1] *tmp,_v[2] *tmp,_v[3] *tmp);
}

template <class T> inline const vec4<T> vec4<T> :: operator *(const T& s) const
{return vec4(_v[0] * s,_v[1] * s,_v[2] * s,_v[3] * s);}

template <class T> inline const vec4<T> vec4<T> :: operator ^(const T& s) const
{return vec4(POW(_v[0],s),POW(_v[1],s),POW(_v[2],s),POW(_v[3],s));}

template <class T> inline const vec4<T>& vec4<T> :: operator +=(const T& s)
{
	_v[0] += s;
	_v[1] += s;
	_v[2] += s;
	_v[3] += s;
	return *this;
}

template <class T> inline const vec4<T>& vec4<T> :: operator -=(const T& s)
{
	_v[0] -= s;
	_v[1] -= s;
	_v[2] -= s;
	_v[3] -= s;
	return *this;
}

template <class T> inline const vec4<T>& vec4<T> :: operator /=(const T& s)
{
	const float tmp = 1 / s;
	_v[0] *= tmp;
	_v[1] *= tmp;
	_v[2] *= tmp;
	_v[3] *= tmp;
	return *this;
}

template <class T> inline const vec4<T>& vec4<T> :: operator *=(const T& s)
{
	_v[0] *= s;
	_v[1] *= s;
	_v[2] *= s;
	_v[3] *= s;
	return *this;
}

template <class T> inline const vec4<T>& vec4<T> :: operator ^=(const T& s)
{
	_v[0] = POW(_v[0],s);
	_v[1] = POW(_v[1],s);
	_v[2] = POW(_v[2],s);
	_v[3] = POW(_v[3],s);
	return *this;
}


// boolean vec4 - vec4
template <class T> const bool vec4<T> :: operator ==(const vec4& v) const
{
	return float_notequal(_v[0],v(0)) 
		? false : float_notequal(_v[1],v(1)) 
		? false : float_notequal(_v[2],v(2))
		? false : float_notequal(_v[3],v(3))
		? false : true;
}

template <class T> const bool vec4<T> :: operator !=(const vec4& v) const
{
	return float_notequal(_v[0],v(0)) 
		? true : float_notequal(_v[1],v(1)) 
		? true : float_notequal(_v[2],v(2))
		? true : float_notequal(_v[3],v(3))
		? true : false;
}

template <class T> inline const bool vec4<T> :: isZero() const
{return (THRESHOLD<T>() > lengthSqr()) ? true : false;}

template <class T> inline const bool vec4<T> :: isUnit() const
{return (THRESHOLD<T>() > (1 - lengthSqr())) ? true : false;}

// utilities
template <class T> inline const T vec4<T> :: dot(const vec4& v) const
{return (_v[0]*v(0) + _v[1]*v(1) + _v[2]*v(2)  + _v[3]*v(3));}

template <class T> inline const vec4<T> vec4<T> :: cross(const vec4& v)	//computes cross & returns it
{return vec4(_v[1]*v(2) - _v[2]*v(1),_v[2]*v(0) - _v[0]*v(2),_v[0]*v(1) - _v[1]*v(0),1);}

template <class T> inline const T vec4<T> :: length() const
{return SQRT<T>(lengthSqr());}

template <class T> inline const T vec4<T> :: lengthSqr() const
{return (_v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2] + _v[3]*_v[3]);}

template <class T> inline const T vec4<T> :: distance(const vec4& v) const
{return SQRT<T>(distanceSqr(v));}

template <class T> inline const T vec4<T> :: distanceSqr(const vec4& v) const
{
	const vec4 vtmp(v - (*this));
	return vtmp.lengthSqr();
}

template <class T> inline const vec4<T> vec4<T> :: normalize()
{
	ASSERT(!isZero());
	const T len_denom = 1 / length();
	_v[0] *= len_denom;
	_v[1] *= len_denom;
	_v[2] *= len_denom;
	_v[3] *= len_denom;
	return *this;
}

template <class T> inline const vec4<T> vec4<T> :: normalize_check()		//checks for unit length first
{
	if(!isUnit())
	{
		ASSERT(!isZero());
		const T len_denom = 1 / length();
		_v[0] *= len_denom;
		_v[1] *= len_denom;
		_v[2] *= len_denom;
		_v[3] *= len_denom;
	}
	return *this;
}

template <class T> inline const vec4<T> vec4<T> :: normalize(const vec4& v) //normalizes v & sets & returns this
{
	ASSERT(!v.isZero());
	const T len_denom = 1 / v.length();
	_v[0] = v(0) * len_denom;
	_v[1] = v(1) * len_denom;
	_v[2] = v(2) * len_denom;
	_v[3] = v(3) * len_denom;
	return *this;
}

// Static members
template <class T> inline unsigned vec4<T> :: size()
{return vec4::_size;}
