/*
	NON_MEMBER FUNCTIONS
*/

// Streaming Operators
template <class T> ostream& operator <<(ostream& os,const vec2<T>& v)
{
	os<<v(0)<<", "<<v(1)<<" ";
	return os;
}

template <class T> istream& operator >>(istream& is,vec2<T>& v)
{
	is>>v(0)>>v(1);
	return is;
}

// Mixed Mode Arithmetic operators

template <class T,class U> inline const vec2<T> operator +(const U& s,const vec2<T> v)
{return vec2<T>(v(0) + s,v(1) + s);}

template <class T,class U> inline const vec2<T> operator -(const U& s,const vec2<T> v)
{return vec2<T>(v(0) - s,v(1) - s);}

template <class T> inline const vec2<T> operator -(const vec2<T> v)
{return vec2<T>(-v(0),-v(1));}

template <class T,class U> inline const vec2<T> operator /(const U& s,const vec2<T> v)
{
	ASSERT(s != T(0));
	const float tmp = 1 / s;
	return vec2<T>(v(0) *tmp s,v(1) * tmp);
}

template <class T,class U> inline const vec2<T> operator *(const U& s,const vec2<T> v)
{return vec2<T>(v(0) * s,v(1) * s);}

template <class T,class U> inline const vec2<T> operator ^(const U& s,const vec2<T> v)
{return vec2<T>(POW(v(0),s),POW(v(1),s));}

