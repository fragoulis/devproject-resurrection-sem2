//*****************************************************************************
//**
//**   File:               HUDRenderer.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#include "HUDRenderer.h"
#include "../GameLogic/GameLogic.h"

HUDRenderer::HUDRenderer() : m_playership(0)
{
}

HUDRenderer::~HUDRenderer()
{
}



void HUDRenderer :: render(Graphics& g) const
{
	// draw energy bars
	// draw number of lives
	// draw type of e-bomb. You could listen to events for e-bomb created/uncreated
	// but asking GameLogic about it is just as easy :)
	// if type is unknown, no ebomb exists
	EbombType ebombType = GameLogic::instance().getCurrentEbombType();
}

void HUDRenderer :: update(float dt)
{
}


void HUDRenderer :: onEvent(Player_Spawned& evt)
{
	m_playership = evt.getValue();
}

void HUDRenderer :: onEvent(Player_Despawned&)
{
	m_playership = 0;
}