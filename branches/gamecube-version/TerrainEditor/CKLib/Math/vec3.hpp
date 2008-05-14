/*
	NON_MEMBER FUNCTIONS
*/

// Streaming Operators
template <class T> ostream& operator <<(ostream& os,const vec3<T>& v)
{
	os<<v(0)<<", "<<v(1)<<", "<<v(2)<<" ";
	return os;
}

template <class T> istream& operator >>(istream& is,vec3<T>& v)
{
	is>>v(0)>>v(1)>>v(2);
	return is;
}

// NonMember Funcs

template <class T>
const vec3<T> normalize(const vec3<T>& v)
{
	vec3<T> vn(v);
	return vn.normalize();
}

// Mixed Mode Arithmetic operators

template <class T,class U> inline const vec3<T> operator +(const U& s,const vec3<T>& v)
{return vec3<T>(v(0) + s,v(1) + s,v(2) + s);}

template <class T,class U> inline const vec3<T> operator -(const U& s,const vec3<T>& v)
{return vec3<T>(v(0) - s,v(1) - s,v(2) - s);}

template <class T> inline const vec3<T> operator -(const vec3<T>& v)
{return vec3<T>(-v(0),-v(1),-v(2));}

template <class T,class U> inline const vec3<T> operator /(const U& s,const vec3<T>& v)
{
	ASSERT(s != T(0));
	const float tmp = 1 / s;
	return vec3<T>(v(0) *tmp s,v(1) * tmp,v(2) *tmp);
}

template <class T,class U> inline const vec3<T> operator *(const U& s,const vec3<T>& v)
{return vec3<T>(v(0) * s,v(1) * s,v(2) * s);}

template <class T,class U> inline const vec3<T> operator ^(const U& s,const vec3<T>& v)
{return vec3<T>(POW(v(0),s),POW(v(1),s),POW(v(2),s));}

