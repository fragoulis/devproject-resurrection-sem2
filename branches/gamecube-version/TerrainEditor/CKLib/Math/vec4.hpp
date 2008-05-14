/*
	NON_MEMBER FUNCTIONS
*/

// Streaming Operators
template <class T> ostream& operator <<(ostream& os,const vec4<T>& v)
{
	os<<v(0)<<", "<<v(1)<<", "<<v(2)<<", "<<v(3)<<" ";
	return os;
}

template <class T> istream& operator >>(istream& is,vec4<T>& v)
{
	is>>v(0)>>v(1)>>v(2)>>v(3);
	return is;
}

// Mixed Mode Arithmetic operators
template <class T,class U> inline const vec4<T> operator +(const U& s,const vec4<T> v)
{return vec4<T>(v(0) + s,v(1) + s,v(2) + s,v(3) + s);}

template <class T,class U> inline const vec4<T> operator -(const U& s,const vec4<T> v)
{return vec4<T>(v(0) - s,v(1) - s,v(2) - s,v(3) - s);}

template <class T> inline const vec4<T> operator -(const vec4<T> v)
{return vec4<T>(-v(0),-v(1),-v(2),-v(3));}

template <class T,class U>
inline const vec4<T> operator /(const U& s,const vec4<T> v)
{
	ASSERT(s != T(0));
	const float tmp = 1 / s;
	return vec4<T>(v(0) *tmp,v(1) *tmp,v(2) *tmp,v(3) *tmp);
}

template <class T,class U> inline const vec4<T> operator *(const U& s,const vec4<T> v)
{return vec4<T>(v(0) * s,v(1) * s,v(2) * s,v(3) * s);}

template <class T,class U> inline const vec4<T> operator ^(const U& s,const vec4<T> v)
{return vec4<T>(POW(v(0),s),POW(v(1),s),POW(v(2),s),POW(v(3),s));}

