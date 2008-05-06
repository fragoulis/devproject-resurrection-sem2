// ctors

inline vec3<float> :: vec3(const float& scalar) 
{_v[0] = _v[1] = _v[2] = scalar;}
inline vec3<float> :: vec3(const float& scalar1,const float& scalar2,const float& scalar3)
{_v[0] = scalar1;_v[1] = scalar2;_v[2] = scalar3;}
inline vec3<float> :: vec3(const vec3<float>& v)
{_v[0] = v(0);_v[1] = v(1);_v[2] = v(2);}
inline const float& vec3<float> :: x() const {return _v[0];}
inline float& vec3<float> :: x() {return _v[0];}
inline const float& vec3<float> :: y() const {return _v[1];}
inline float& vec3<float> :: y() {return _v[1];}
inline const float& vec3<float> :: z() const {return _v[2];}
inline float& vec3<float> :: z() {return _v[2];}
inline const float& vec3<float> :: w() const {return _v[3];}
inline float& vec3<float> :: w() {return _v[3];}

// access
 inline const float& vec3<float> :: operator ()(const unsigned i) const 
{ASSERT(i < 4);return _v[i];}
 inline float& vec3<float> :: operator()(const unsigned i)
{ASSERT(i < 4);return _v[i];}
 inline const float * const vec3<float> :: get() const 
{return static_cast<const float *>(_v);}

// arithmetic vec3 - vec3
 inline const vec3<float> vec3<float> :: operator +(const vec3& v) const
{return vec3(_v[0] + v(0),_v[1] + v(1),_v[2] + v(2));}

 inline const vec3<float> vec3<float> :: operator -(const vec3& v) const
{return vec3(_v[0] - v(0),_v[1] - v(1),_v[2] - v(2));}

 inline const vec3<float> vec3<float> :: operator /(const vec3& v) const
{return vec3(_v[0]/v(0),_v[1]/v(1),_v[2]/v(2));}

 inline const vec3<float> vec3<float> :: operator *(const vec3& v) const
{return vec3(_v[0]*v(0),_v[1]*v(1),_v[2]*v(2));}

 inline const vec3<float> vec3<float> :: operator ^(const vec3& v) const
{return vec3(POW(_v[0],v(0)),POW(_v[1],v(1)),POW(_v[2],v(2)));}

 inline const vec3<float>& vec3<float> :: operator +=(const vec3& v)
{	
	_simd = _mm_add_ps(_simd,v.simd());// *** MUCH FASfloatER !!!
	return *this;
}

 inline const vec3<float>& vec3<float> :: operator -=(const vec3& v)
{
	_simd = _mm_sub_ps(_simd,v.simd());
	return *this;
}

 inline const vec3<float>& vec3<float> :: operator /=(const vec3& v)
{
	_v[0] /= v(0);
	_v[1] /= v(1);
	_v[2] /= v(2);
	return *this;
}

 inline const vec3<float>& vec3<float> :: operator *=(const vec3& v)
{
	_v[0] *= v(0);
	_v[1] *= v(1);
	_v[2] *= v(2);
	return *this;
}

 inline const vec3<float>& vec3<float> :: operator ^=(const vec3& v)
{
	_v[0] = POW(_v[0],v(0));
	_v[1] = POW(_v[1],v(1));
	_v[2] = POW(_v[2],v(2));
	return *this;
}

 inline const vec3<float>& vec3<float> :: operator =(const vec3& v)
{
	_v[0] = v(0);
	_v[1] = v(1);
	_v[2] = v(2);	
	// FIXME : (SIMD code) simdval = _mm_load_ps(v.get());
	return *this;
}

// arithmetic vec3 - scalar

 inline const vec3<float> vec3<float> :: operator +(const float& s) const
{return vec3(_v[0] + s,_v[1] + s,_v[2] + s);}

 inline const vec3<float> vec3<float> :: operator -(const float& s) const
{return vec3(_v[0] - s,_v[1] - s,_v[2] - s);}

 inline const vec3<float> vec3<float> :: operator /(const float& s) const
{
	ASSERT(s != float(0));
	const float tmp = 1 / s;
	return vec3(_v[0] *tmp,_v[1] *tmp,_v[2] *tmp);
}

 inline const vec3<float> vec3<float> :: operator *(const float& s) const
{return vec3(_v[0] * s,_v[1] * s,_v[2] * s);}

 inline const vec3<float> vec3<float> :: operator ^(const float& s) const
{return vec3(POW(_v[0],s),POW(_v[1],s),POW(_v[2],s));}

 inline const vec3<float>& vec3<float> :: operator +=(const float& s)
{_v[0] += s;_v[1] += s;_v[2] += s;return *this;}

 inline const vec3<float>& vec3<float> :: operator -=(const float& s)
{_v[0] -= s;_v[1] -= s;_v[2] -= s;return *this;}

 inline const vec3<float>& vec3<float> :: operator /=(const float& s)
{
	ASSERT(s != float(0));
	const float tmp = 1 / s;
	_v[0] *= tmp;
	_v[1] *= tmp;
	_v[2] *= tmp;
	return *this;
}

 inline const vec3<float>& vec3<float> :: operator *=(const float& s)
{
	_v[0] *= s;
	_v[1] *= s;
	_v[2] *= s;
	return *this;
}

 inline const vec3<float>& vec3<float> :: operator ^=(const float& s)
{
	_v[0] = POW(_v[0],s);
	_v[1] = POW(_v[1],s);
	_v[2] = POW(_v[2],s);
	return *this;
}


// boolean vec3 - vec3
 const bool vec3<float> :: operator ==(const vec3& v) const
{
	return float_notequal(_v[0],v(0)) 
		? false : float_notequal(_v[1],v(1)) 
		? false : float_notequal(_v[2],v(2))
		? false : true;
}

 const bool vec3<float> :: operator !=(const vec3& v) const
{
	return float_notequal(_v[0],v(0)) 
		? true : float_notequal(_v[1],v(1)) 
		? true : float_notequal(_v[2],v(2))
		? true : false;
}

 inline const bool vec3<float> :: isZero() const
{return (THRESHOLD<float>() > lengthSqr()) ? true : false;}
 inline const bool vec3<float> :: isUnit() const
{return (THRESHOLD<float>() > (1 - lengthSqr())) ? true : false;}

// utilities

inline const float vec3<float> :: dot(const vec3& v) const
{return (_v[0]*v(0) + _v[1]*v(1) + _v[2]*v(2));}

 inline const vec3<float> vec3<float> :: cross(const vec3& v) const
{return vec3<float>(_v[1]*v(2) - _v[2]*v(1),_v[2]*v(0) - _v[0]*v(2),_v[0]*v(1) - _v[1]*v(0));}

 inline const float vec3<float> :: length() const
{return SQRT<float>(lengthSqr());}

 inline const float vec3<float> :: lengthSqr() const
{return (_v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2]);}

 inline const float vec3<float> :: distance(const vec3& v) const
{return SQRT<float>(distanceSqr(v));}

 inline const float vec3<float> :: distanceSqr(const vec3& v) const
{const vec3 vtmp(v - (*this)); return vtmp.lengthSqr();}

 inline const vec3<float> vec3<float> :: normalize()
{
	ASSERT(!isZero());
	const float len_denom = 1 / length();
	_v[0] *= len_denom;
	_v[1] *= len_denom;
	_v[2] *= len_denom;
	return *this;
}

 inline const vec3<float> vec3<float> :: normalize_check()		//checks for unit length first
{
	if(!isUnit())
	{
		ASSERT(!isZero());
		const float len_denom = 1 / length();
		_v[0] *= len_denom;
		_v[1] *= len_denom;
		_v[2] *= len_denom;
	}
	return *this;
}

 inline const vec3<float> vec3<float> :: normalize(const vec3& v) //normalizes v & sets & returns this
{
	ASSERT(!v.isZero());
	const float len_denom = 1 / v.length();
	_v[0] = v(0) * len_denom;
	_v[1] = v(1) * len_denom;
	_v[2] = v(2) * len_denom;
	return *this;
}

// Static members
 inline unsigned vec3<float> :: size()
{return vec3::_size;}
