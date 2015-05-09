#pragma once

#include "FireCube.h"

class MathUtils
{
public:
	static bool ClosestPointsOnTwoLines(FireCube::vec3 &closestPointLine1, FireCube::vec3 linePoint1, FireCube::vec3 lineVec1, FireCube::vec3 linePoint2, FireCube::vec3 lineVec2);
};
