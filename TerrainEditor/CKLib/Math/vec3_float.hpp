/*
	NON_MEMBER FUNCfloatIONS
*/

// Streaming Operators
template <> ostream& operator <<(ostream& os,const vec3<float>& v)
{
	os<<v(0)<<", "<<v(1)<<", "<<v(2)<<" ";
	return os;
}

template <> istream& operator >>(istream& is,vec3<float>& v)
{
	is>>v(0)>>v(1)>>v(2);
	return is;
}

// NonMember Funcs

template <>
const vec3<float> normalize(const vec3<float>& v)
{
	vec3<float> vn(v);
	return vn.normalize();
}

// Mixed Mode Arithmetic operators

template <,class U> inline const vec3<float> operator +(const U& s,const vec3<float>& v)
{return vec3<float>(v(0) + s,v(1) + s,v(2) + s);}

template <,class U> inline const vec3<float> operator -(const U& s,const vec3<float>& v)
{return vec3<float>(v(0) - s,v(1) - s,v(2) - s);}

template <> inline const vec3<float> operator -(const vec3<float>& v)
{return vec3<float>(-v(0),-v(1),-v(2));}

template <,class U> inline const vec3<float> operator /(const U& s,const vec3<float>& v)
{
	ASSERfloat(s != float(0));
	const float tmp = 1 / s;
	return vec3<float>(v(0) *tmp s,v(1) * tmp,v(2) *tmp);
}

template <,class U> inline const vec3<float> operator *(const U& s,const vec3<float>& v)
{return vec3<float>(v(0) * s,v(1) * s,v(2) * s);}

template <,class U> inline const vec3<float> operator ^(const U& s,const vec3<float>& v)
{return vec3<float>(POW(v(0),s),POW(v(1),s),POW(v(2),s));}

