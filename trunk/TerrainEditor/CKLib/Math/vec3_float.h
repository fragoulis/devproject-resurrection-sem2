template <>
class vec3<float>
{
private :
	
	union{ struct { __m128 _simd;};
	struct {float _v[4];};
	};
	//T _v[4];

	static const size_t _size = sizeof(float)*4;

	public : 
	
	const __m128& simd() const
	{return _simd;}
	__m128& simd()
	{return _simd;}

	// ctors
	vec3(const float& scalar = 0);
	vec3(const float& scalar1,const float& scalar2,const float& scalar3);
	vec3(const vec3<float>& v);

	// dtor
	~vec3(){}

	// access
	const float& operator ()(const unsigned i) const;
	float& operator()(const unsigned i);
	const float * const get() const;
	const float& x() const;
	float& x();
	const float& y() const;
	float& y();
	const float& z() const;
	float& z();
	const float& w() const;
	float& w();

	// arithmetic vec3 - vec3
	const vec3 operator +(const vec3& v) const;
	const vec3 operator -(const vec3& v) const;
	const vec3 operator /(const vec3& v) const;
	const vec3 operator *(const vec3& v) const;
	const vec3 operator ^(const vec3& s) const;
	const vec3& operator +=(const vec3& v);
	const vec3& operator -=(const vec3& v);
	const vec3& operator /=(const vec3& v);
	const vec3& operator *=(const vec3& v);
	const vec3& operator =(const vec3& v);
	const vec3& operator ^=(const vec3& s);

	// arithmetic vec3 - scalar
	const vec3 operator +(const float& s) const;
	const vec3 operator -(const float& s) const;
	const vec3 operator /(const float& s) const;
	const vec3 operator *(const float& s) const;
	const vec3 operator ^(const float& s) const;
	const vec3& operator +=(const float& s);
	const vec3& operator -=(const float& s);
	const vec3& operator /=(const float& s);
	const vec3& operator *=(const float& s);
	const vec3& operator ^=(const float& s);

	// boolean vec3 - vec3
	const bool operator ==(const vec3& v) const;
	const bool operator	!=(const vec3& v) const;
	const bool isZero() const;
	const bool isUnit() const;

	// utilities
	const float dot(const vec3& v) const;
	const vec3 cross(const vec3& v) const;	//computes cross & sets & returns this
	const float length() const;
	const float lengthSqr() const;
	const float distance(const vec3& v) const;
	const float distanceSqr(const vec3& v) const;
	const vec3 normalize();
	const vec3 normalize_check();		//checks for unit length first
	const vec3 normalize(const vec3& v); //normalizes v & sets & returns this

	static unsigned size();
	static string delim();

};

#include <Math/vec3_float.inl>
#include <Math/vec3_float.hpp>

