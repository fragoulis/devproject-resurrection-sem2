template<class T>
unsigned short math_const<T> :: counter(0);

template <class T>
T math_const<T> :: _rand_list[65536];

template <class T>
unsigned math_const<T> :: _randseed(0);

template <class T>
inline void math_const<T> :: init_rand(const unsigned seed)
{
	_randseed = (seed)?seed : GetTickCount();
	srand(_randseed);
	for(unsigned i=0;i<65536;++i)
		_rand_list[i] = rand() *RANDMAX_TO_1;
}

template <class T>
inline const unsigned math_const<T> :: rand_seed()
{return _randseed;}