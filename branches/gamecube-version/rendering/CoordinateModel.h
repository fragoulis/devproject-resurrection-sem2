#ifndef _RES_COORDINATE_MODEL_H_
#define _RES_COORDINATE_MODEL_H_

/*
	Helper class storing model & coordinate frame pointers
*/

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


#endif