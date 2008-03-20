//*****************************************************************************
//**
//**   File:               Line.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#include "Line.h"
#include "CoordinateFrame.h"

void Line::transform(const CoordinateFrame& cf)
{
	cf.transform(m_point);
	cf.transform(m_vector);
}

void Line::reverseTransform(const CoordinateFrame& cf)
{
	cf.reverseTransform(m_point);
	cf.reverseTransform(m_vector);
}
