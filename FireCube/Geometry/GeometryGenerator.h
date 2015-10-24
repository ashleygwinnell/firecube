#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include "Geometry/Geometry.h"

namespace FireCube
{

class Engine;

/**
* A namespace containing various functions to generate geometries.
*/
namespace GeometryGenerator
{
/**
* Generates a box.
* @param size The extents of the box.
* @return The generated geometry.
*/
Geometry *FIRECUBE_API GenerateBox(Engine *engine, const vec3 &size);

/**
* Generates a sphere.
* @param radius The radius of the sphere.
* @param rings The number of rings in the sphere.
* @param columns The number of columns in the sphere.
* @return The generated geometry.
*/
Geometry *FIRECUBE_API GenerateSphere(Engine *engine, float radius, unsigned int rings, unsigned int columns);

/**
* Generates a plane.
* @param size The extents of the plane.
* @return The generated geometry.
*/
Geometry *FIRECUBE_API GeneratePlane(Engine *engine, const vec2 &size);

Geometry *FIRECUBE_API GenerateTorus(Engine *engine, float outerRadius, float innerRadius, float endAngle, unsigned int segments, unsigned int tubeSegments);

Geometry *FIRECUBE_API GenerateCylinder(Engine *engine, float bottomRadius, float topRadius, float height, unsigned int heightSegments, unsigned int radialSegments);

}
}
#pragma warning(pop)
