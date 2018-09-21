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

/**
* Generates a torus
* @param outerRadius The outer radius of the torus
* @param innerRadius The inner radius of the torus
* @param endAngle The end angle of the torus
* @param segments The number of of ring segments
* @param tubeSegments The number of faces to create for the tube
*/
Geometry *FIRECUBE_API GenerateTorus(Engine *engine, float outerRadius, float innerRadius, float endAngle, unsigned int segments, unsigned int tubeSegments);

/**
* Generates a cylinder
* @param bottomRadius The bottom radius of the cylinder
* @param topRadius The top radius of the cylinder
* @param height The height of the cylinder
* @param heightSegments The number of segments aloong the cylinder main axis
* @param radialSegments The number of segments revolving the cylinder
*/
Geometry *FIRECUBE_API GenerateCylinder(Engine *engine, float bottomRadius, float topRadius, float height, unsigned int heightSegments, unsigned int radialSegments);

/**
* Generates a 3d polyline
* @param polyline The polyline (a list of 3d points)
*/
Geometry *FIRECUBE_API GeneratePolyline(Engine *engine, const std::vector<vec3> &polyline);

/**
* Generates a grid.
* @param size The extents of the grid.
* @param countX The number of subdivisions along the x axis
* @param countZ The number of subdivisions along the z axis
* @return The generated geometry.
*/
Geometry *FIRECUBE_API GenerateGrid(Engine *engine, const vec2 &size, unsigned int countX, unsigned int countZ);

}
}
#pragma warning(pop)
