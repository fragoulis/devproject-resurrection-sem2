/*
	INVERSE SPECIFIC
*/

template <class T> const T matrix44<T> :: _det3(const T *smat)
{
	return (smat[0] * ( smat[4]*smat[8] - smat[7]*smat[5] )
		- smat[1] * ( smat[3]*smat[8] - smat[6]*smat[5] )
		+ smat[2] * ( smat[3]*smat[7] - smat[6]*smat[4] ));
}

template <class T> void matrix44<T> :: _extract_submat(T *mb, const int i, const int j ) 
{
	int di, dj, si, sj;
	// loop through 3x3 submatrix
	for( di = 0; di < 3; di ++ ) 
	{
		for( dj = 0; dj < 3; dj ++ ) 
		{
			// map 3x3 element (destination) to 4x4 element (source)
			si = di + ( ( di >= i ) ? 1 : 0 );
			sj = dj + ( ( dj >= j ) ? 1 : 0 );
			// copy element
			mb[di * 3 + dj] = _rows(si)(sj);
		}
	}
}

template <class T> const T matrix44<T> :: determinant4()
{
	T result = 0, i = 1;
	T msub3[9];
	unsigned n;
	for ( n = 0; n < 4; n++, i *= T(-1.0) )
	{
		_extract_submat(msub3, 0, n );
		result += _rows(0)(n) * _det3(msub3) * i;
	}
	return result;
}

template <class T> const matrix44<T>& matrix44<T> :: inverse()
{
	matrix44<T> mat;
	const float  mdet = determinant4();
	if(mdet < THRESHOLD<T>())
	{
		ASSERT(1);
		loadIdentity();
	}
	const float mdet_denom = 1/mdet;
	float mtemp[9];
	int i, j, sign;
	for ( i = 0; i < 4; i++ )
		for ( j = 0; j < 4; j++ )
		{
			sign = 1 - ( (i +j) % 2 ) * 2;
			_extract_submat( mtemp, i, j );
			mat(j,i) = ( _det3(mtemp ) * sign ) * mdet_denom;
		}
	*(const_cast<matrix44<T> *>(this)) = mat;
	return *this;
}


/*
	NON_MEMBER FUNCTIONS
*/

// Streaming operators
template <class T> ostream& operator <<(ostream& os,const matrix44<T>& m)
{
	os<<m.getRow4(0)<<m.getRow4(1)<<m.getRow4(2)<<m.getRow4(3);
	return os;
}

template <class T> istream& operator >>(istream& is,matrix44<T>& m)
{
	is>>m.getRow4(0)>>m.getRow4(1)>>m.getRow4(2)>>m.getRow4(3);
	return is;
}