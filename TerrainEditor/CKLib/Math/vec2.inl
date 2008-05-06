// ctors

template <class T> inline vec2<T> :: vec2(const T& scalar) 
{_v[0] = _v[1] = scalar;}
template <class T> inline vec2<T> :: vec2(const T& scalar1,const T& scalar2)
{_v[0] = scalar1;_v[1] = scalar2;}
template <class T> inline vec2<T> :: vec2(const vec2<T>& v)
{_v[0] = v(0);_v[1] = v(1);}
template <class T> inline const T& vec2<T> :: x() const {return _v[0];}
template <class T> inline T& vec2<T> :: x() {return _v[0];}
template <class T> inline const T& vec2<T> :: y() const {return _v[1];}
template <class T> inline T& vec2<T> :: y() {return _v[1];}

// access
template <class T> inline const T& vec2<T> :: operator ()(const unsigned i) const 
{ASSERT(i < 4);return _v[i];}
template <class T> inline T& vec2<T> :: operator()(const unsigned i)
{ASSERT(i < 4);return _v[i];}
template <class T> inline const T * const vec2<T> :: get() const 
{return static_cast<const T *>(_v);}

// arithmetic vec2 - vec2
template <class T> inline const vec2<T> vec2<T> :: operator +(const vec2& v) const
{return vec2(_v[0] + v(0),_v[1] + v(1));}

template <class T> inline const vec2<T> vec2<T> :: operator -(const vec2& v) const
{return vec2(_v[0] - v(0),_v[1] - v(1));}

template <class T> inline const vec2<T> vec2<T> :: operator /(const vec2& v) const
{return vec2(_v[0]/v(0),_v[1]/v(1));}

template <class T> inline const vec2<T> vec2<T> :: operator *(const vec2& v) const
{return vec2(_v[0]*v(0),_v[1]*v(1));}

template <class T> inline const vec2<T> vec2<T> :: operator ^(const vec2& v) const
{return vec2(POW(_v[0],v(0)),POW(_v[1],v(1)));}

template <class T> inline const vec2<T>& vec2<T> :: operator +=(const vec2& v)
{	_v[0] += v(0);
	_v[1] += v(1);
	// _simd = _mm_add_ps(_simd,v.simd()); *** MUCH FASTER !!!
	return *this;
}

template <class T> inline const vec2<T>& vec2<T> :: operator -=(const vec2& v)
{
	_v[0] -= v(0);
	_v[1] -= v(1);
	return *this;
}

template <class T> inline const vec2<T>& vec2<T> :: operator /=(const vec2& v)
{
	_v[0] /= v(0);
	_v[1] /= v(1);
	return *this;
}

template <class T> inline const vec2<T>& vec2<T> :: operator *=(const vec2& v)
{
	_v[0] *= v(0);
	_v[1] *= v(1);
	return *this;
}

template <class T> inline const vec2<T>& vec2<T> :: operator ^=(const vec2& v)
{
	_v[0] = POW(_v[0],v(0));
	_v[1] = POW(_v[1],v(1));
	return *this;
}

template <class T> inline const vec2<T>& vec2<T> :: operator =(const vec2& v)
{
	_v[0] = v(0);
	_v[1] = v(1);
	return *this;
}

// arithmetic vec2 - scalar

template <class T> inline const vec2<T> vec2<T> :: operator +(const T& s) const
{return vec2(_v[0] + s,_v[1] + s);}

template <class T> inline const vec2<T> vec2<T> :: operator -(const T& s) const
{return vec2(_v[0] - s,_v[1] - s);}

template <class T> inline const vec2<T> vec2<T> :: operator /(const T& s) const
{
	ASSERT(s != T(0));
	const T tmp = 1 / s;
	return vec2(_v[0] *tmp,_v[1] *tmp);
}

template <class T> inline const vec2<T> vec2<T> :: operator *(const T& s) const
{return vec2(_v[0] * s,_v[1] * s);}

template <class T> inline const vec2<T> vec2<T> :: operator ^(const T& s) const
{return vec2(POW(_v[0],s),POW(_v[1],s));}

template <class T> inline const vec2<T>& vec2<T> :: operator +=(const T& s)
{_v[0] += s;_v[1] += s;return *this;}

template <class T> inline const vec2<T>& vec2<T> :: operator -=(const T& s)
{_v[0] -= s;_v[1] -= s;return *this;}

template <class T> inline const vec2<T>& vec2<T> :: operator /=(const T& s)
{
	ASSERT(s != T(0))
	const float tmp = 1 / s;
	_v[0] *= s;
	_v[1] *= s;
	return *this;
}

template <class T> inline const vec2<T>& vec2<T> :: operator *=(const T& s)
{
	_v[0] *= s;
	_v[1] *= s;
	return *this;
}

template <class T> inline const vec2<T>& vec2<T> :: operator ^=(const T& s)
{
	_v[0] = POW(_v[0],s);
	_v[1] = POW(_v[1],s);
	return *this;
}


// boolean vec2 - vec2
template <class T> const bool vec2<T> :: operator ==(const vec2& v) const
{
	return float_notequal(_v[0],v(0)) 
		? false : float_notequal(_v[1],v(1)) 
		? false : true;
}

template <class T> const bool vec2<T> :: operator !=(const vec2& v) const
{
	return float_notequal(_v[0],v(0)) 
		? true : float_notequal(_v[1],v(1)) 
		? true : false;
}

template <class T> inline const bool vec2<T> :: isZero() const
{return (THRESHOLD<T>() > lengthSqr()) ? true : false;}
template <class T> inline const bool vec2<T> :: isUnit() const
{return (THRESHOLD<T>() > (1 - lengthSqr())) ? true : false;}

// utilities
template <class T>
inline const T vec2<T> :: dot(const vec2& v) const
{return (_v[0]*v(0) + _v[1]*v(1));}

template <class T> inline const T vec2<T> :: length() const
{return SQRT<T>(lengthSqr());}

template <class T> inline const T vec2<T> :: lengthSqr() const
{return (_v[0]*_v[0] + _v[1]*_v[1]);}

template <class T> inline const T vec2<T> :: distance(const vec2& v) const
{return SQRT<T>(distanceSqr(v));}

template <class T> inline const T vec2<T> :: distanceSqr(const vec2& v) const
{const vec2 vtmp(v - (*this)); return vtmp.lengthSqr();}

template <class T> inline const vec2<T> vec2<T> :: normalize()
{
	ASSERT(!isZero());
	const T len_denom = 1 / length();
	_v[0] *= len_denom;
	_v[1] *= len_denom;
	return *this;
}

template <class T> inline const vec2<T> vec2<T> :: normalize_check()		//checks for unit length first
{
	if(!isUnit())
	{
		ASSERT(!isZero());
		const T len_denom = 1 / length();
		_v[0] *= len_denom;
		_v[1] *= len_denom;
	}
	return *this;
}

template <class T> inline const vec2<T> vec2<T> :: normalize(const vec2& v) //normalizes v & sets & returns this
{
	ASSERT(!v.isZero());
	const T len_denom = 1 / v.length();
	_v[0] = v(0) * len_denom;
	_v[1] = v(1) * len_denom;
	return *this;
}

// Static members
template <class T> inline unsigned vec2<T> :: size()
{return vec2::_size;}
