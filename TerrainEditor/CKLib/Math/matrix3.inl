template <class T> inline matrix33<T> :: matrix33(const T& a)
{
	_rows(0)(0) = _rows(1)(1) = _rows(2)(2) = a;
	_rows(0)(1) = _rows(0)(2) = 
		_rows(1)(0) = _rows(1)(2) = 
		_rows(2)(0) = _rows(2)(1) = T(0.0);
}
template <class T> inline matrix33<T> :: matrix33()		// Identity
{
	_rows(0)(0) = _rows(1)(1) = _rows(2)(2) =T(1.0);
	_rows(0)(1) = _rows(0)(2) = 
		_rows(1)(0) = _rows(1)(2) = 
		_rows(2)(0) = _rows(2)(1) = T(0.0);
}

template <class T> inline matrix33<T> :: matrix33(const matrix33& mat)
:_rows(mat.getRow3(0),mat.getRow3(1),mat.getRow3(2))
{}


template <class T> inline matrix33<T> :: matrix33(const T mat[9])
:_rows( vec3<T>(mat[0],mat[1],mat[2]),
	    vec3<T>(mat[3],mat[4],mat[5]),
	    vec3<T>(mat[6],mat[7],mat[8]))	   
{}


template <class T> inline matrix33<T> :: matrix33(const T& a1,const T& a2,const T& a3,const T& a4,
							 const T& a5,const T& a6,const T& a7,const T& a8,
							 const T& a9)
:_rows(vec3<T>(a1,a2,a3),
		vec3<T>(a4,a5,a6),
		vec3<T>(a7,a8,a9))
{}

// access 
template <class T> inline const T& matrix33<T> :: operator ()(const unsigned& i, const unsigned& j) const
{return _rows(i)(j);}

template <class T> inline const vec3<T>& matrix33<T> :: getRow3(const unsigned& i) const
{return _rows(i);}

template <class T> inline const T * const matrix33<T> :: get() const
{return _rows(0).get();}

template <class T> inline T& matrix33<T> :: operator ()(const unsigned& i, const unsigned& j)
{return _rows(i)(j);}

template <class T> inline vec3<T>& matrix33<T> :: getRow3(const unsigned& i)
{return _rows(i);}


// arithmetic
template <class T> inline const matrix33<T> matrix33<T> :: operator * (const matrix33<T>& mat) const
{
	return matrix33<T>(_rows(0)(0)*mat(0,0) + _rows(0)(1)*mat(1,0) + _rows(0)(2)*mat(2,0),
					   _rows(0)(0)*mat(0,1) + _rows(0)(1)*mat(1,1) + _rows(0)(2)*mat(2,1),
					   _rows(0)(0)*mat(0,2) + _rows(0)(1)*mat(1,2) + _rows(0)(2)*mat(2,2),
					   _rows(1)(0)*mat(0,0) + _rows(1)(1)*mat(1,0) + _rows(1)(2)*mat(2,0),
					   _rows(1)(0)*mat(0,1) + _rows(1)(1)*mat(1,1) + _rows(1)(2)*mat(2,1),
					   _rows(1)(0)*mat(0,2) + _rows(1)(1)*mat(1,2) + _rows(1)(2)*mat(2,2),
					   _rows(2)(0)*mat(0,0) + _rows(2)(1)*mat(1,0) + _rows(2)(2)*mat(2,0),
					   _rows(2)(0)*mat(0,1) + _rows(2)(1)*mat(1,1) + _rows(2)(2)*mat(2,1),
					   _rows(2)(0)*mat(0,2) + _rows(2)(1)*mat(1,2) + _rows(2)(2)*mat(2,2));
}

template <class T> inline const matrix33<T>& matrix33<T> :: operator = (const matrix33<T>& mat)
{
	_rows(0) = mat.getRow3(0);
	_rows(1) = mat.getRow3(1);
	_rows(2) = mat.getRow3(2);
	return *this;
}

template <class T> inline const matrix33<T>& matrix33<T> :: operator *= (const matrix33& mat)
{
	*(const_cast<matrix33<T> *>(this)) = matrix33<T>(
		   _rows(0)(0)*mat(0,0) + _rows(0)(1)*mat(1,0) + _rows(0)(2)*mat(2,0),
		   _rows(0)(0)*mat(0,1) + _rows(0)(1)*mat(1,1) + _rows(0)(2)*mat(2,1),
		   _rows(0)(0)*mat(0,2) + _rows(0)(1)*mat(1,2) + _rows(0)(2)*mat(2,2),
		   _rows(1)(0)*mat(0,0) + _rows(1)(1)*mat(1,0) + _rows(1)(2)*mat(2,0),
		   _rows(1)(0)*mat(0,1) + _rows(1)(1)*mat(1,1) + _rows(1)(2)*mat(2,1),
		   _rows(1)(0)*mat(0,2) + _rows(1)(1)*mat(1,2) + _rows(1)(2)*mat(2,2),
		   _rows(2)(0)*mat(0,0) + _rows(2)(1)*mat(1,0) + _rows(2)(2)*mat(2,0),
		   _rows(2)(0)*mat(0,1) + _rows(2)(1)*mat(1,1) + _rows(2)(2)*mat(2,1),
		   _rows(2)(0)*mat(0,2) + _rows(2)(1)*mat(1,2) + _rows(2)(2)*mat(2,2));

	return *this;
}

template <class T> inline const vec3<T> matrix33<T> :: operator * (const vec3<T>& v) const
{
	return vec3<T>(_rows(0)(0)*v(0) + _rows(1)(0)*v(1) + _rows(2)(0)*v(2),
				   _rows(0)(1)*v(0) + _rows(1)(1)*v(1) + _rows(2)(1)*v(2),
				   _rows(0)(2)*v(0) + _rows(1)(2)*v(1) + _rows(2)(2)*v(2));
}

// boolean

template <class T> const bool matrix33<T> :: operator == (const matrix33& mat)
{
	return (_rows(0) != mat.getRow3(0)) 
		? false : (_rows(1) != mat.getRow3(1)) 
		? false : (_rows(2) != mat.getRow3(2))
		? false : true;
}

template <class T> const bool matrix33<T> :: operator != (const matrix33& mat)
{
	return (_rows(0) != mat.getRow3(0)) 
		? true : (_rows(1) != mat.getRow3(1)) 
		? true : (_rows(2) != mat.getRow3(2)) 
		? true : false;
}

template <class T> const bool matrix33<T> :: isZero() const
{
	return (_rows(0).lengthSqr() > THRESHOLD<T>())
		? false : (_rows(1).lengthSqr() > THRESHOLD<T>())
		? false : (_rows(2).lengthSqr() > THRESHOLD<T>())
		? false : true;

}

template <class T> const bool matrix33<T> :: isIdentity() const
{
	return (_rows(0) != vec3f(T(1),T(0),T(0)))
		? false : (_rows(1) != vec3f(T(0),T(1),T(0)))
		? false : (_rows(2) != vec3f(T(0),T(0),T(1)))
		? false : true;

}


//utilities
template <class T> inline const matrix33<T>& matrix33<T> :: loadZero()
{
	_rows = vec3<vec3<T> >(T(0));
	return *this;
}

template <class T> inline const matrix33<T>& matrix33<T> :: loadIdentity()
{
	_rows(0)(0) = _rows(1)(1) = _rows(2)(2) = T(1.0);
		_rows(0)(1) = _rows(0)(2) =
		_rows(1)(0) = _rows(1)(2) = 
		_rows(2)(0) = _rows(2)(1) =  T(0.0);

	return *this;
}

template <class T> const matrix33<T>& matrix33<T> :: transpose()
{
	swap(_rows(0)(1),_rows(1)(0));
	swap(_rows(0)(2),_rows(2)(0));
	swap(_rows(1)(2),_rows(2)(1));
	return *this;
}


template <class T> inline const size_t matrix33<T> :: size()
{return _size;}