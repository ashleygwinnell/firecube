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
GeometryPtr FIRECUBE_API GenerateBox(Engine *engine, const vec3 &size);

/**
* Generates a box.
* @param size The extents of the box.
* @param material The material to assign to the geometry.
* @return The generated geometry.
*/
GeometryPtr FIRECUBE_API GenerateBox(Engine *engine, const vec3 &size, MaterialPtr material);

/**
* Generates a sphere.
* @param radius The radius of the sphere.
* @param rings The number of rings in the sphere.
* @param columns The number of columns in the sphere.
* @return The generated geometry.
*/
GeometryPtr FIRECUBE_API GenerateSphere(Engine *engine, float radius, unsigned int rings, unsigned int columns);

/**
* Generates a sphere.
* @param radius The radius of the sphere.
* @param rings The number of rings in the sphere.
* @param columns The number of columns in the sphere.
* @param material The material to assign to the geometry.
* @return The generated geometry.
*/
GeometryPtr FIRECUBE_API GenerateSphere(Engine *engine, float radius, unsigned int rings, unsigned int columns, MaterialPtr material);

/**
* Generates a plane.
* @param size The extents of the plane.
* @return The generated geometry.
*/
GeometryPtr FIRECUBE_API GeneratePlane(Engine *engine, const vec2 &size);

/**
* Generates a plane.
* @param size The extents of the plane.
* @param material The material to assign to the geometry.
* @return The generated geometry.
*/
GeometryPtr FIRECUBE_API GeneratePlane(Engine *engine, const vec2 &size, MaterialPtr material);
}
}
#pragma warning(pop)
