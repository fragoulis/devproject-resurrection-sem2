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
