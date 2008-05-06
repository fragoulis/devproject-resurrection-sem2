// ctors

template <class T> inline vec3<T> :: vec3(const T& scalar) 
{_v[0] = _v[1] = _v[2] = scalar;}
template <class T> inline vec3<T> :: vec3(const T& scalar1,const T& scalar2,const T& scalar3)
{_v[0] = scalar1;_v[1] = scalar2;_v[2] = scalar3;}
template <class T> inline vec3<T> :: vec3(const vec3<T>& v)
{_v[0] = v(0);_v[1] = v(1);_v[2] = v(2);}
template <class T> inline const T& vec3<T> :: x() const {return _v[0];}
template <class T> inline T& vec3<T> :: x() {return _v[0];}
template <class T> inline const T& vec3<T> :: y() const {return _v[1];}
template <class T> inline T& vec3<T> :: y() {return _v[1];}
template <class T> inline const T& vec3<T> :: z() const {return _v[2];}
template <class T> inline T& vec3<T> :: z() {return _v[2];}
template <class T> inline const T& vec3<T> :: w() const {return _v[3];}
template <class T> inline T& vec3<T> :: w() {return _v[3];}

template <class T> void assign(const T x,const T y,const T z)
{
	_v[0] = x;
	_v[1] = y;
	_v[2] = z;
	_v[3] = T(1.0);
}

// access
template <class T> inline const T& vec3<T> :: operator ()(const unsigned i) const 
{ASSERT(i < 4);return _v[i];}
template <class T> inline T& vec3<T> :: operator()(const unsigned i)
{ASSERT(i < 4);return _v[i];}
template <class T> inline const T * const vec3<T> :: get() const 
{return static_cast<const T *>(_v);}

template <class T> inline void vec3<T> :: assign(const T x,const T y,const T z)
{_v[0] = x;_v[1] = y;_v[2] = z;}

// arithmetic vec3 - vec3
template <class T> inline const vec3<T> vec3<T> :: operator +(const vec3& v) const
{return vec3(_v[0] + v(0),_v[1] + v(1),_v[2] + v(2));}

template <class T> inline const vec3<T> vec3<T> :: operator -(const vec3& v) const
{return vec3(_v[0] - v(0),_v[1] - v(1),_v[2] - v(2));}

template <class T> inline const vec3<T> vec3<T> :: operator /(const vec3& v) const
{return vec3(_v[0]/v(0),_v[1]/v(1),_v[2]/v(2));}

template <class T> inline const vec3<T> vec3<T> :: operator *(const vec3& v) const
{return vec3(_v[0]*v(0),_v[1]*v(1),_v[2]*v(2));}

template <class T> inline const vec3<T> vec3<T> :: operator ^(const vec3& v) const
{return vec3(POW(_v[0],v(0)),POW(_v[1],v(1)),POW(_v[2],v(2)));}

template <class T> inline const vec3<T>& vec3<T> :: operator +=(const vec3& v)
{	_v[0] += v(0);
	_v[1] += v(1);
	_v[2] += v(2);
	// _simd = _mm_add_ps(_simd,v.simd()); *** MUCH FASTER !!!
	return *this;
}

template <class T> inline const vec3<T>& vec3<T> :: operator -=(const vec3& v)
{
	_v[0] -= v(0);
	_v[1] -= v(1);
	_v[2] -= v(2);
	return *this;
}

template <class T> inline const vec3<T>& vec3<T> :: operator /=(const vec3& v)
{
	_v[0] /= v(0);
	_v[1] /= v(1);
	_v[2] /= v(2);
	return *this;
}

template <class T> inline const vec3<T>& vec3<T> :: operator *=(const vec3& v)
{
	_v[0] *= v(0);
	_v[1] *= v(1);
	_v[2] *= v(2);
	return *this;
}

template <class T> inline const vec3<T>& vec3<T> :: operator ^=(const vec3& v)
{
	_v[0] = POW(_v[0],v(0));
	_v[1] = POW(_v[1],v(1));
	_v[2] = POW(_v[2],v(2));
	return *this;
}

template <class T> inline const vec3<T>& vec3<T> :: operator =(const vec3& v)
{
	_v[0] = v(0);
	_v[1] = v(1);
	_v[2] = v(2);	
	// FIXME : (SIMD code) simdval = _mm_load_ps(v.get());
	return *this;
}

// arithmetic vec3 - scalar

template <class T> inline const vec3<T> vec3<T> :: operator +(const T& s) const
{return vec3(_v[0] + s,_v[1] + s,_v[2] + s);}

template <class T> inline const vec3<T> vec3<T> :: operator -(const T& s) const
{return vec3(_v[0] - s,_v[1] - s,_v[2] - s);}

template <class T> inline const vec3<T> vec3<T> :: operator /(const T& s) const
{
	ASSERT(s != T(0));
	const T tmp = 1 / s;
	return vec3(_v[0] *tmp,_v[1] *tmp,_v[2] *tmp);
}

template <class T> inline const vec3<T> vec3<T> :: operator *(const T& s) const
{return vec3(_v[0] * s,_v[1] * s,_v[2] * s);}

template <class T> inline const vec3<T> vec3<T> :: operator ^(const T& s) const
{return vec3(POW(_v[0],s),POW(_v[1],s),POW(_v[2],s));}

template <class T> inline const vec3<T>& vec3<T> :: operator +=(const T& s)
{_v[0] += s;_v[1] += s;_v[2] += s;return *this;}

template <class T> inline const vec3<T>& vec3<T> :: operator -=(const T& s)
{_v[0] -= s;_v[1] -= s;_v[2] -= s;return *this;}

template <class T> inline const vec3<T>& vec3<T> :: operator /=(const T& s)
{
	ASSERT(s != T(0));
	const float tmp = 1 / s;
	_v[0] *= tmp;
	_v[1] *= tmp;
	_v[2] *= tmp;
	return *this;
}

template <class T> inline const vec3<T>& vec3<T> :: operator *=(const T& s)
{
	_v[0] *= s;
	_v[1] *= s;
	_v[2] *= s;
	return *this;
}

template <class T> inline const vec3<T>& vec3<T> :: operator ^=(const T& s)
{
	_v[0] = POW(_v[0],s);
	_v[1] = POW(_v[1],s);
	_v[2] = POW(_v[2],s);
	return *this;
}


// boolean vec3 - vec3
template <class T> const bool vec3<T> :: operator ==(const vec3& v) const
{
	return float_notequal(_v[0],v(0)) 
		? false : float_notequal(_v[1],v(1)) 
		? false : float_notequal(_v[2],v(2))
		? false : true;
}

template <class T> const bool vec3<T> :: operator !=(const vec3& v) const
{
	return float_notequal(_v[0],v(0)) 
		? true : float_notequal(_v[1],v(1)) 
		? true : float_notequal(_v[2],v(2))
		? true : false;
}

template <class T> inline const bool vec3<T> :: isZero() const
{return (THRESHOLD<T>() > lengthSqr()) ? true : false;}
template <class T> inline const bool vec3<T> :: isUnit() const
{return (THRESHOLD<T>() > (1 - lengthSqr())) ? true : false;}

// utilities
template <class T>
inline const T vec3<T> :: dot(const vec3& v) const
{return (_v[0]*v(0) + _v[1]*v(1) + _v[2]*v(2));}

template <class T> inline const vec3<T> vec3<T> :: cross(const vec3& v) const
{return vec3<T>(_v[1]*v(2) - _v[2]*v(1),_v[2]*v(0) - _v[0]*v(2),_v[0]*v(1) - _v[1]*v(0));}

template <class T> inline const T vec3<T> :: length() const
{return SQRT<T>(lengthSqr());}

template <class T> inline const T vec3<T> :: lengthSqr() const
{return (_v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2]);}

template <class T> inline const T vec3<T> :: distance(const vec3& v) const
{return SQRT<T>(distanceSqr(v));}

template <class T> inline const T vec3<T> :: distanceSqr(const vec3& v) const
{const vec3 vtmp(v - (*this)); return vtmp.lengthSqr();}

template <class T> inline const vec3<T> vec3<T> :: normalize()
{
	const float lensqr = lengthSqr();
	if(lensqr > THRESHOLD<float>())
	{
		const T len_denom = 1 / SQRT<T>(lensqr);
		_v[0] *= len_denom;
		_v[1] *= len_denom;
		_v[2] *= len_denom;
	}
	return *this;
}

template <class T> inline const vec3<T> vec3<T> :: normalize_check()		//checks for unit length first
{
	if(!isUnit())
	{
		ASSERT(!isZero());
		const T len_denom = 1 / length();
		_v[0] *= len_denom;
		_v[1] *= len_denom;
		_v[2] *= len_denom;
	}
	return *this;
}

template <class T> inline const vec3<T> vec3<T> :: normalize(const vec3& v) //normalizes v & sets & returns this
{
	ASSERT(!v.isZero());
	const T len_denom = 1 / v.length();
	_v[0] = v(0) * len_denom;
	_v[1] = v(1) * len_denom;
	_v[2] = v(2) * len_denom;
	return *this;
}

template <class T> inline const vec3<T> vec3<T> :: abs()  const
{
	return vec3<T>(ABS(_v[0]),ABS(_v[1]),ABS(_v[2]));
}

// Static members
template <class T> inline unsigned vec3<T> :: size()
{return vec3::_size;}
