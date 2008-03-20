//*****************************************************************************
//**
//**   File:               LineSegment.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#include "LineSegment.h"
#include "CoordinateFrame.h"

void LineSegment::transform(const CoordinateFrame& cf)
{
	cf.transform(m_point1);
	cf.transform(m_point2);
}

void LineSegment::reverseTransform(const CoordinateFrame& cf)
{
	cf.reverseTransform(m_point1);
	cf.reverseTransform(m_point2);
}
