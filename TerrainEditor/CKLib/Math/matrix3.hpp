/*
	INVERSE SPECIFIC
*/

template < class T>
const T matrix33<T> :: determinant3()
{
    return( _rows(0)(0) * ( _rows(1)(1)*_rows(2)(2) - _rows(2)(1)*_rows(1)(2) )
          - _rows(0)(1) * ( _rows(1)(0)*_rows(2)(2) - _rows(2)(0)*_rows(1)(2) )
          + _rows(0)(2) * ( _rows(1)(0)*_rows(2)(1) - _rows(2)(0)*_rows(1)(1) ));
}

template <class T> const matrix33<T>& matrix33<T> :: inverse()
{
	float det = determinant3();
     if ( fabs( det ) < 0.0005f )
       {
		   assert(0);
       }

	 const float detdenom = 1.0f / det;

	 matrix33<T> mat(
      _rows(1)(1)*_rows(2)(2) - _rows(1)(2)*_rows(2)(1)   *detdenom,		// 0,0
   -( _rows(1)(0)*_rows(2)(2) - _rows(1)(2)*_rows(2)(0) ) *detdenom,		// 0,1
	  _rows(1)(0)*_rows(2)(1) - _rows(2)(0)*_rows(1)(1)   *detdenom,		// 0,2
   -( _rows(0)(1)*_rows(2)(2) - _rows(2)(1)*_rows(0)(2) ) *detdenom,		// 1,0
      _rows(0)(0)*_rows(2)(2) - _rows(2)(0)*_rows(0)(2)   *detdenom,		// 1,1
   -( _rows(0)(0)*_rows(2)(1) - _rows(2)(0)*_rows(0)(1) ) *detdenom,		// 1,2
	  _rows(0)(1)*_rows(1)(2) - _rows(1)(1)*_rows(0)(2)   *detdenom,		// 2,0
   -( _rows(0)(0)*_rows(1)(2) - _rows(1)(0)*_rows(0)(2) ) *detdenom,		// 2,1
      _rows(0)(0)*_rows(1)(1) - _rows(0)(1)*_rows(1)(0)   *detdenom);		// 2,2

	 *this = mat;

     return *this;
}


/*
	NON_MEMBER FUNCTIONS
*/

// Streaming operators
template <class T> ostream& operator <<(ostream& os,const matrix33<T>& m)
{
	os<<m.getRow3(0)<<m.getRow3(1)<<m.getRow3(2);
	return os;
}

template <class T> istream& operator >>(istream& is,matrix33<T>& m)
{
	is>>m.getRow3(0)>>m.getRow3(1)>>m.getRow3(2);
	return is;
}