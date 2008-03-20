//*****************************************************************************
//**
//**   File:               CoordinateModel.h
//**   Author:             $Author: esrever2357 $
//**   Last Change:        $Date: 2008-03-11 05:50:47 +0000 (Tue, 11 Mar 2008) $
//**   Last Revision:      $Revision: 134 $
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
