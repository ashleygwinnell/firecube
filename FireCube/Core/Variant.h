#pragma once

#include "Utils/utils.h"
#include "Math/Math.h"

namespace FireCube
{

enum class FIRECUBE_API VariantType
{
	NONE, INT, FLOAT, BOOL, VEC2, VEC3, VEC4
};

class VariantValue
{
public:
	union
	{
		int int0;
		float float0;
		bool  bool0;
	};

	union
	{
		int int1;
		float float1;		
	};

	union
	{
		int int2;
		float float2;		
	};

	union
	{
		int int3;
		float float3;		
	};
};

class FIRECUBE_API Variant
{
public:
	Variant();
	Variant(int v);
	Variant(float v);
	Variant(bool v);
	Variant(const vec2 &v);
	Variant(const vec3 &v);
	Variant(const vec4 &v);

	Variant &operator = (int v);
	Variant &operator = (float v);
	Variant &operator = (bool v);
	Variant &operator = (const vec2 &v);
	Variant &operator = (const vec3 &v);
	Variant &operator = (const vec4 &v);

	VariantType GetType() const;

	int GetInt() const;
	float GetFloat() const;
	bool GetBool() const;
	const vec2 &GetVec2() const;
	const vec3 &GetVec3() const;
	const vec4 &GetVec4() const;

	static Variant FromString(const std::string &v);
	std::string ToString() const;
private:
	VariantType type;
	VariantValue value;
};

}