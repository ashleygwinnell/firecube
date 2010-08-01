#ifndef GEOMETRY_GENERATOR_H
#define GEOMETRY_GENERATOR_H

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{
/**
* A namespace containing various functions to generate geometries.
*/
namespace GeometryGenerator
{
	Geometry FIRECUBE_API GenerateBox(vec3 size);
	Geometry FIRECUBE_API GenerateBox(vec3 size,Material material);
	Geometry FIRECUBE_API GenerateSphere(float radius,unsigned int rings, unsigned int columns);
	Geometry FIRECUBE_API GenerateSphere(float radius,unsigned int rings, unsigned int columns,Material material);
	Geometry FIRECUBE_API GeneratePlane(vec2 size);
	Geometry FIRECUBE_API GeneratePlane(vec2 size,Material material);
}
}
#pragma warning(pop)

#endif