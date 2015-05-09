#include "MathUtils.h"
using namespace FireCube;

bool MathUtils::ClosestPointsOnTwoLines(vec3 &closestPointLine1, vec3 linePoint1, vec3 lineVec1, vec3 linePoint2, vec3 lineVec2)
{
	float a = Dot(lineVec1, lineVec1);
	float b = Dot(lineVec1, lineVec2);
	float e = Dot(lineVec2, lineVec2);
	float d = a*e - b*b;

	//lines are not parallel
	if (d != 0.0f)
	{
		vec3 r = linePoint1 - linePoint2;
		float c = Dot(lineVec1, r);
		float f = Dot(lineVec2, r);
		float s = (b*f - c*e) / d;
		float t = (a*f - c*b) / d;
		closestPointLine1 = linePoint1 + lineVec1 * s;
		return true;
	}
	else
	{
		return false;
	}
}
