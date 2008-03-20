//*****************************************************************************
//**
//**   File:               ISerializable.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
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
