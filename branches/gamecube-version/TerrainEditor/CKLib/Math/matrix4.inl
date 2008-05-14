template <class T> inline matrix44<T> :: matrix44()		// Identity
{
	_rows(0)(0) = _rows(1)(1) = _rows(2)(2) = _rows(3)(3) = T(1.0);
	_rows(0)(1) = _rows(0)(2) = _rows(0)(3) =
		_rows(1)(0) = _rows(1)(2) = _rows(0)(3) =
		_rows(2)(0) = _rows(2)(1) = _rows(2)(3) =
		_rows(3)(0) = _rows(3)(1) = _rows(3)(2) = T(0.0);
}

template <class T> inline matrix44<T> :: matrix44(const matrix44& mat)
:_rows(mat.getRow4(0),mat.getRow4(1),mat.getRow4(2),mat.getRow4(3))
{}


template <class T> inline matrix44<T> :: matrix44(const T mat[16])
:_rows( vec4<T>(mat[0],mat[1],mat[2],mat[3]),
	    vec4<T>(mat[4],mat[5],mat[6],mat[7]),
	    vec4<T>(mat[8],mat[9],mat[10],mat[11]),
	    vec4<T>(mat[12],mat[13],mat[14],mat[15]))	   
{}


template <class T> inline matrix44<T> :: matrix44(const T& a1,const T& a2,const T& a3,const T& a4,
							 const T& a5,const T& a6,const T& a7,const T& a8,
							 const T& a9,const T& a10,const T& a11,const T& a12,
							 const T& a13,const T& a14,const T& a15,const T& a16)
:_rows(vec4<T>(a1,a2,a3,a4),
		vec4<T>(a5,a6,a7,a8),
		vec4<T>(a9,a10,a11,a12),
		vec4<T>(a13,a14,a15,a16))
{}

template <class T> inline matrix44<T> :: matrix44(const vec4<T>& trans)		// set only an  iden + translation
{
	_rows(0)(0) = _rows(1)(1) = _rows(2)(2) = T(1.0);
	_rows(0)(1) = _rows(0)(2) = _rows(0)(3) =
		_rows(1)(0) = _rows(1)(2) = _rows(0)(3) =
		_rows(2)(0) = _rows(2)(1) = _rows(2)(3) = T(0.0);
	_rows(3) = trans;
}

template <class T> inline matrix44<T> :: matrix44(const vec3<T>& trans)		// set only a translation
{
	_rows(0)(0) = _rows(1)(1) = _rows(2)(2) = T(1.0);
	_rows(0)(1) = _rows(0)(2) = _rows(0)(3) =
		_rows(1)(0) = _rows(1)(2) = _rows(0)(3) =
		_rows(2)(0) = _rows(2)(1) = _rows(2)(3) = T(0.0);
	_rows(3) = vec4<T>(trans(0),trans(1),trans(2),T(1.0));
}

// access 
template <class T> inline const T& matrix44<T> :: operator ()(const unsigned& i, const unsigned& j) const
{return _rows(i)(j);}

template <class T> inline const vec3<T>& matrix44<T> :: getRow3(const unsigned& i) const
{return reinterpret_cast<const vec3<T>&>(_rows(i));}

template <class T> inline const vec4<T>& matrix44<T> :: getRow4(const unsigned& i) const
{return _rows(i);}

template <class T> inline const T * const matrix44<T> :: get() const
{return _rows(0).get();}

template <class T> inline T& matrix44<T> :: operator ()(const unsigned& i, const unsigned& j)
{return _rows(i)(j);}

template <class T> inline vec3<T>& matrix44<T> :: getRow3(const unsigned& i)
{return reinterpret_cast<vec3<T>&>(_rows(i));}

template <class T> inline vec4<T>& matrix44<T> :: getRow4(const unsigned& i)
{return _rows(i);}

// arithmetic
template <class T> inline const matrix44<T> matrix44<T> :: operator * (const matrix44<T>& mat) const
{
	return matrix44<T>(_rows(0)(0)*mat(0,0) + _rows(0)(1)*mat(1,0) + _rows(0)(2)*mat(2,0) + _rows(0)(3)*mat(3,0),
					   _rows(0)(0)*mat(0,1) + _rows(0)(1)*mat(1,1) + _rows(0)(2)*mat(2,1) + _rows(0)(3)*mat(3,1),
					   _rows(0)(0)*mat(0,2) + _rows(0)(1)*mat(1,2) + _rows(0)(2)*mat(2,2) + _rows(0)(3)*mat(3,2),
					   _rows(0)(0)*mat(0,3) + _rows(0)(1)*mat(1,3) + _rows(0)(2)*mat(2,3) + _rows(0)(3)*mat(3,3),
					   _rows(1)(0)*mat(0,0) + _rows(1)(1)*mat(1,0) + _rows(1)(2)*mat(2,0) + _rows(1)(3)*mat(3,0),
					   _rows(1)(0)*mat(0,1) + _rows(1)(1)*mat(1,1) + _rows(1)(2)*mat(2,1) + _rows(1)(3)*mat(3,1),
					   _rows(1)(0)*mat(0,2) + _rows(1)(1)*mat(1,2) + _rows(1)(2)*mat(2,2) + _rows(1)(3)*mat(3,2),
					   _rows(1)(0)*mat(0,3) + _rows(1)(1)*mat(1,3) + _rows(1)(2)*mat(2,3) + _rows(1)(3)*mat(3,3),
					   _rows(2)(0)*mat(0,0) + _rows(2)(1)*mat(1,0) + _rows(2)(2)*mat(2,0) + _rows(2)(3)*mat(3,0),
					   _rows(2)(0)*mat(0,1) + _rows(2)(1)*mat(1,1) + _rows(2)(2)*mat(2,1) + _rows(2)(3)*mat(3,1),
					   _rows(2)(0)*mat(0,2) + _rows(2)(1)*mat(1,2) + _rows(2)(2)*mat(2,2) + _rows(2)(3)*mat(3,2),
					   _rows(2)(0)*mat(0,3) + _rows(2)(1)*mat(1,3) + _rows(2)(2)*mat(2,3) + _rows(2)(3)*mat(3,3),
					   _rows(3)(0)*mat(0,0) + _rows(3)(1)*mat(1,0) + _rows(3)(2)*mat(2,0) + _rows(3)(3)*mat(3,0),
					   _rows(3)(0)*mat(0,1) + _rows(3)(1)*mat(1,1) + _rows(3)(2)*mat(2,1) + _rows(3)(3)*mat(3,1),
					   _rows(3)(0)*mat(0,2) + _rows(3)(1)*mat(1,2) + _rows(3)(2)*mat(2,2) + _rows(3)(3)*mat(3,2),
					   _rows(3)(0)*mat(0,3) + _rows(3)(1)*mat(1,3) + _rows(3)(2)*mat(2,3) + _rows(3)(3)*mat(3,3));
}

template <class T> inline const matrix44<T>& matrix44<T> :: operator = (const matrix44<T>& mat)
{
	_rows(0) = mat.getRow4(0);
	_rows(1) = mat.getRow4(1);
	_rows(2) = mat.getRow4(2);
	_rows(3) = mat.getRow4(3);
	return *this;
}

template <class T> inline const matrix44<T>& matrix44<T> :: operator *= (const matrix44& mat)
{
	*(const_cast<matrix44<T> *>(this)) = matrix44<T>(
		   _rows(0)(0)*mat(0,0) + _rows(0)(1)*mat(1,0) + _rows(0)(2)*mat(2,0) + _rows(0)(3)*mat(3,0),
		   _rows(0)(0)*mat(0,1) + _rows(0)(1)*mat(1,1) + _rows(0)(2)*mat(2,1) + _rows(0)(3)*mat(3,1),
		   _rows(0)(0)*mat(0,2) + _rows(0)(1)*mat(1,2) + _rows(0)(2)*mat(2,2) + _rows(0)(3)*mat(3,2),
		   _rows(0)(0)*mat(0,3) + _rows(0)(1)*mat(1,3) + _rows(0)(2)*mat(2,3) + _rows(0)(3)*mat(3,3),
		   _rows(1)(0)*mat(0,0) + _rows(1)(1)*mat(1,0) + _rows(1)(2)*mat(2,0) + _rows(1)(3)*mat(3,0),
		   _rows(1)(0)*mat(0,1) + _rows(1)(1)*mat(1,1) + _rows(1)(2)*mat(2,1) + _rows(1)(3)*mat(3,1),
		   _rows(1)(0)*mat(0,2) + _rows(1)(1)*mat(1,2) + _rows(1)(2)*mat(2,2) + _rows(1)(3)*mat(3,2),
		   _rows(1)(0)*mat(0,3) + _rows(1)(1)*mat(1,3) + _rows(1)(2)*mat(2,3) + _rows(1)(3)*mat(3,3),
		   _rows(2)(0)*mat(0,0) + _rows(2)(1)*mat(1,0) + _rows(2)(2)*mat(2,0) + _rows(2)(3)*mat(3,0),
		   _rows(2)(0)*mat(0,1) + _rows(2)(1)*mat(1,1) + _rows(2)(2)*mat(2,1) + _rows(2)(3)*mat(3,1),
		   _rows(2)(0)*mat(0,2) + _rows(2)(1)*mat(1,2) + _rows(2)(2)*mat(2,2) + _rows(2)(3)*mat(3,2),
		   _rows(2)(0)*mat(0,3) + _rows(2)(1)*mat(1,3) + _rows(2)(2)*mat(2,3) + _rows(2)(3)*mat(3,3),
		   _rows(3)(0)*mat(0,0) + _rows(3)(1)*mat(1,0) + _rows(3)(2)*mat(2,0) + _rows(3)(3)*mat(3,0),
		   _rows(3)(0)*mat(0,1) + _rows(3)(1)*mat(1,1) + _rows(3)(2)*mat(2,1) + _rows(3)(3)*mat(3,1),
		   _rows(3)(0)*mat(0,2) + _rows(3)(1)*mat(1,2) + _rows(3)(2)*mat(2,2) + _rows(3)(3)*mat(3,2),
		   _rows(3)(0)*mat(0,3) + _rows(3)(1)*mat(1,3) + _rows(3)(2)*mat(2,3) + _rows(3)(3)*mat(3,3));
}

template <class T> inline const vec3<T> matrix44<T> :: operator * (const vec3<T>& v) const
{
	return vec3<T>(_rows(0)(0)*v(0) + _rows(1)(0)*v(1) + _rows(2)(0)*v(2) + _rows(3)(0),
				   _rows(0)(1)*v(0) + _rows(1)(1)*v(1) + _rows(2)(1)*v(2) + _rows(3)(1),
				   _rows(0)(2)*v(0) + _rows(1)(2)*v(1) + _rows(2)(2)*v(2) + _rows(3)(2));
}

template <class T> inline const vec4<T> matrix44<T> :: operator * (const vec4<T>& v) const
{
	return vec4<T>(_rows(0)(0)*v(0) + _rows(1)(0)*v(1) + _rows(2)(0)*v(2) + _rows(3)(0)*v(3),
				   _rows(0)(1)*v(0) + _rows(1)(1)*v(1) + _rows(2)(1)*v(2) + _rows(3)(1)*v(3),
				   _rows(0)(2)*v(0) + _rows(1)(2)*v(1) + _rows(2)(2)*v(2) + _rows(3)(2)*v(3),
				   _rows(0)(3)*v(0) + _rows(1)(3)*v(1) + _rows(2)(3)*v(2) + _rows(3)(3)*v(3));
}

// boolean

template <class T> const bool matrix44<T> :: operator == (const matrix44& mat)
{
	return (_rows(0) != mat(0)) 
		? false : (_rows(1) != mat(1)) 
		? false : (_rows(2) != mat(2)) 
		? false : (_rows(3) != mat(3))
		? false : true;
}

template <class T> const bool matrix44<T> :: operator != (const matrix44& mat)
{
	return (_rows(0) != mat(0)) 
		? true : (_rows(1) != mat(1)) 
		? true : (_rows(2) != mat(2)) 
		? true : (_rows(3) != mat(3))
		? true : false;
}

template <class T> const bool matrix44<T> :: isZero() const
{
	return (_rows(0).lengthSqr() > THRESHOLD<T>())
		? false : (_rows(1).lengthSqr() > THRESHOLD<T>())
		? false : (_rows(2).lengthSqr() > THRESHOLD<T>())
		? false : (_rows(3).lengthSqr() > THRESHOLD<T>())
		? false : true;

}

template <class T> const bool matrix44<T> :: isIdentity() const
{
	return (_rows(0) != vec4f(T(1),T(0),T(0),T(0)))
		? false : (_rows(1) != vec4f(T(0),T(1),T(0),T(0)))
		? false : (_rows(2) != vec4f(T(0),T(0),T(1),T(0)))
		? false : (_rows(3) != vec4f(T(0),T(0),T(0),T(1)))
		? false : true;

}


//utilities
template <class T> inline const matrix44<T>& matrix44<T> :: loadZero()
{
	_rows = vec4<vec4<T> >(T(0));
	return *this;
}

template <class T> inline const matrix44<T>& matrix44<T> :: loadIdentity()
{
	_rows(0)(0) = _rows(1)(1) = _rows(2)(2) = _rows(3)(3) = T(1.0);
		_rows(0)(1) = _rows(0)(2) = _rows(0)(3) =
		_rows(1)(0) = _rows(1)(2) = _rows(0)(3) =
		_rows(2)(0) = _rows(2)(1) = _rows(2)(3) =
		_rows(3)(0) = _rows(3)(1) = _rows(3)(2) = T(0.0);
	return *this;
}

template <class T> const matrix44<T>& matrix44<T> :: transpose()
{
	swap(_rows(0)(1),_rows(1)(0));
	swap(_rows(0)(2),_rows(2)(0));
	swap(_rows(0)(3),_rows(3)(0));
	swap(_rows(1)(2),_rows(2)(1));
	swap(_rows(1)(3),_rows(3)(1));
	swap(_rows(2)(3),_rows(3)(2));
	return *this;
}

template <class T> const vec3<T> matrix44<T> :: rotate(const vec3<T>& v)
{
	return vec3<T>(_rows(0)(0)*v(0) + _rows(1)(0)*v(1) + _rows(2)(0)*v(2),
				   _rows(0)(1)*v(0) + _rows(1)(1)*v(1) + _rows(2)(1)*v(2),
				   _rows(0)(2)*v(0) + _rows(1)(2)*v(1) + _rows(2)(2)*v(2));
}

template <class T> inline const size_t matrix44<T> :: size()
{return _size;}