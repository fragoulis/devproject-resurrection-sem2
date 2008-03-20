//*****************************************************************************
//**
//**   File:               Exception.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#pragma once
#include <string>


class Exception
{
public:
	Exception() : m_text("Unknown exception") { }
	Exception(const std::string& text) : m_text(text) { }
	virtual ~Exception() { }

	std::string getText() const { return m_text; }
	void setText(const std::string& text) { m_text = text; }

private:
	std::string m_text;
};
