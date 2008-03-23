//*****************************************************************************
//**
//**   File:               WorldObject.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 16:22:19 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 127 $
//**
//*****************************************************************************

#include "WorldObject.h"
#include "../gfxutils/ConfParser/ParserSection.h"
#include "../gfxutils/Misc/utils.h"
#include "../math/Point2.h"

WorldObject :: WorldObject()
{
}

WorldObject :: ~WorldObject()
{
}

void WorldObject :: loadSettings( const ParserSection& ps )
{
	Point2 pos;
	pos = FromString<Point2>(ps.getVal("Location"));
	m_coordinateFrame.setX(pos.getX());
	m_coordinateFrame.setZ(pos.getY());
}
