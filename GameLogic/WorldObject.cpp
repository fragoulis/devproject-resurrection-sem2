//*****************************************************************************
//**
//**   File:               WorldObject.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#include "WorldObject.h"
#include "../gfxutils/ConfParser/ParserSection.h"
#include "../gfxutils/Misc/utils.h"
#include "../math/Vector3.h"

WorldObject :: WorldObject()
{
}

WorldObject :: ~WorldObject()
{
}

void WorldObject :: loadSettings( const ParserSection& ps )
{
	Vector3 position;
	position = FromString<Vector3>(ps.getVal("Position"));
	m_coordinateFrame.setOrigin(position);
}
