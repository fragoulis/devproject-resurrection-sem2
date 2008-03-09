//*****************************************************************************
//**
//**   File:               ISerializable.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#pragma once
#include <iosfwd>

// An object that overloads streaming operators is serializable

class ISerializable
{
public:
	ISerializable();
	virtual ~ISerializable();

	virtual std::istream& read(std::istream& is) = 0;
	virtual std::ostream& write(std::ostream& os) const { return os; } // easier to code quickly :)
};




inline std::istream& operator >> (std::istream& is, ISerializable& s)
{
	return s.read(is);
}

inline std::ostream& operator << (std::ostream& os, const ISerializable& s)
{
	return s.write(os);
}
