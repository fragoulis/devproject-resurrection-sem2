// Linear mix function, the percentage is that of the first value over the second
template <class T>
inline T mix(const T& v1,const T& v2,const float perc)
{
	// clamp to 0-1
	const float newperc = (perc > 1.0f) ? 1.0f : (perc < 0) ? 0 : perc;
	return v2*newperc + v1*(1.0f - newperc);
}

// Type to string function
template<class T>
std::string ToString(const T& val)
{
    std::ostringstream str;
    str << val;
	return str.str();
}

// String to type function
template<class T>
T FromString(const std::string& strval)
{
	T val;
	std::stringstream str(strval);
    str>>val;
	return val;
	
}

//template<>
//bool FromString<bool>(const std::string& strval)
//{
//	if (strval == "true") return true;
//	return false;
//}