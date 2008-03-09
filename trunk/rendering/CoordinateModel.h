//*****************************************************************************
//**
//**   File:               CoordinateModel.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#pragma once

class Model;
class CoordinateFrame;
struct CoordinateModel
{
	const Model * model;
	const CoordinateFrame * coordframe;
	CoordinateModel(const Model * mod,const CoordinateFrame * cframe)
		:model(mod),coordframe(cframe){}
	const CoordinateModel& operator = (const CoordinateModel& cm)
	{
		model = cm.model;
		coordframe=cm.coordframe;
		return *this;
	}
};