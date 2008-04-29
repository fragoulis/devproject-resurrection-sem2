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
