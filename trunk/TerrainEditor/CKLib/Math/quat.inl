// ctor
template <class T> inline quat<T> :: quat ()
{
	_vec[0] = _vec[1] = _vec[2] = T(0.0);
	_vec[3] = T(1.0);
}

template <class T> inline quat<T> :: quat (const T& x,const T& y,const T& z,const T& w)
{
	_vec[0] = x;
	_vec[1] = y;
	_vec[2] = z;
	_vec[3] = w;
}

// access
template <class T> inline const T& quat<T> :: x() const {return _vec[0];}
template <class T> inline T& quat<T> :: x() {return _vec[0];}
template <class T> inline const T& quat<T> :: y() const {return _vec[1];}
template <class T> inline T& quat<T> :: y() {return _vec[1];}
template <class T> inline const T& quat<T> :: z() const {return _vec[2];}
template <class T> inline T& quat<T> :: z() {return _vec[2];}
template <class T> inline const T& quat<T> :: w() const {return _vec[3];}
template <class T> inline T& quat<T> :: w() {return _vec[3];}

// operators
template <class T> inline const quat<T> quat<T> :: operator *(const T& s) const
{return quat(_vec[0],_vec[1],_vec[2],_vec[3]*s);}

template <class T> const quat<T> quat<T> :: operator *(const quat& q) const
{
	 return quat(w() * q.x() + x() * q.w() + y() * q.z() - z() * q.y(),
				 w() * q.y() - x() * q.z() + y() * q.w() + z() * q.x(),
				 w() * q.z() + x() * q.y() - y() * q.x() + z() * q.w(),
				 w() * q.w() - x() * q.x() - y() * q.y() - z() * q.z());
}

template <class T> const quat<T> quat<T> :: operator +(const quat& q) const
{
	return quat(x() + q.x(),y() + q.y(),z() + q.z(),w() + q.w());
}

template <class T> inline const quat<T> quat<T> :: operator ~() const
{return quat(-_vec[0],-_vec[1],-_vec[2],_vec[3]);}

template <class T> const quat<T>& quat<T> :: operator *=(const T& s)
{_vec[3] *= s;return *this;}

template <class T> const quat<T>& quat<T> :: operator *=(const quat& q)
{
	quat qnew(w() * q.x() + x() * q.w() + y() * q.z() - z() * q.y(),
				 w() * q.y() - x() * q.z() + y() * q.w() + z() * q.x(),
				 w() * q.z() + x() * q.y() - y() * q.x() + z() * q.w(),
				 w() * q.w() - x() * q.x() - y() * q.y() - z() * q.z());
	_vec[0] = qnew.x();
	_vec[1] = qnew.y();
	_vec[2] = qnew.z();
	_vec[3] = qnew.w();
	return *this;
}

// utilities

template <class T> void quat<T> :: normalize()
{
	//static unsigned norms = 0, needed = 0;
	//++norms;
	const T sql = sqLength();
	if (sql  > THRESHOLD<T>())
	{
		T len = 1.0f/SQRT(sql);
		_vec[0] *= len;
		_vec[1] *= len;
		_vec[2] *= len;
		_vec[3] *= len;
		//++needed;
	}
	//std::cout<<"Total : "<<needed<<" out of "<<norms<<std::endl;
}

template <class T> inline const T quat<T> :: length() const
{return SQRT(length());}

template <class T> inline const T quat<T> :: sqLength() const
{return (_vec[0]*_vec[0] + _vec[1]*_vec[1] + _vec[2]*_vec[2] + _vec[3]*_vec[3]);}

template <class T>
const matrix44<T> quat<T> :: toMatrix() const
{
	 // Calculate the first row.
	return matrix44<T> (
						1.0f - 2.0f * (y() * y() + z() * z()),
						2.0f * (x() * y() + z() * w()),
						2.0f * (x() * z() - y() * w()),
						0.0f,
     // Calculate the second row().
						2.0f * (x() * y() - z() * w()),
						1.0f - 2.0f * (x() * x() + z() * z()),
						2.0f * (z() * y() + x() * w()),
						0.0f,
     // Calculate the third row().
						2.0f * (x() * z() + y() * w()),
						2.0f * (y() * z() - x() * w()),
						1.0f - 2.0f * (x() * x() + y() * y()),
						0.0f,
	 // Calculate the fourth row().
						0.0f,
						0.0f,  
						0.0f,  
						1.0f);
}

template <class T>
quat<T> quat<T> :: nlerp(const quat<T>& qsrc,const quat<T>& qdst,const float t)
{
	quat<T> q(qsrc*t + qdst*(T(1) - t));
	q.normalize();
	return q;
}