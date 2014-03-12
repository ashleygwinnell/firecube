#include "Variant.h"

using namespace FireCube;

Variant::Variant() : type(VariantType::NONE)
{

}

Variant::Variant(int v)
{
	*this = v;
}

Variant::Variant(float v)
{
	*this = v;
}

Variant::Variant(bool v)
{
	*this = v;
}

Variant::Variant(const vec2 &v)
{
	*this = v;
}

Variant::Variant(const vec3 &v)
{
	*this = v;
}

Variant::Variant(const vec4 &v)
{
	*this = v;
}

Variant &Variant::operator = (int v)
{
	type = VariantType::INT;
	value.int0 = v;
	return *this;
}

Variant &Variant::operator = (float v)
{
	type = VariantType::FLOAT;
	value.float0 = v;
	return *this;
}

Variant &Variant::operator = (bool v)
{
	type = VariantType::BOOL;
	value.bool0 = v;
	return *this;
}

Variant &Variant::operator = (const vec2 &v)
{
	type = VariantType::VEC2;
	*(reinterpret_cast<vec2 *>(&value)) = v;
	return *this;
}

Variant &Variant::operator = (const vec3 &v)
{
	type = VariantType::VEC3;
	*(reinterpret_cast<vec3 *>(&value)) = v;
	return *this;
}

Variant &Variant::operator = (const vec4 &v)
{
	type = VariantType::VEC4;
	*(reinterpret_cast<vec4 *>(&value)) = v;
	return *this;
}

VariantType Variant::GetType() const
{
	return type;
}

int Variant::GetInt() const
{
	return type == VariantType::INT ? value.int0 : 0;
}

float Variant::GetFloat() const
{
	return type == VariantType::FLOAT ? value.float0 : 0.0f;
}

bool Variant::GetBool() const
{
	return type == VariantType::BOOL ? value.bool0 : false;
}


const vec2 &Variant::GetVec2() const
{
	return type == VariantType::VEC2 ? *(reinterpret_cast<const vec2 *>(&value)) : vec2::ZERO;
}

const vec3 &Variant::GetVec3() const
{
	return type == VariantType::VEC3 ? *(reinterpret_cast<const vec3 *>(&value)) : vec3::ZERO;
}

const vec4 &Variant::GetVec4() const
{
	return type == VariantType::VEC4 ? *(reinterpret_cast<const vec4 *>(&value)) : vec4::ZERO;
}

Variant Variant::FromString(const std::string &v)
{
	Variant ret;
	if (isalpha(v[0]))
	{
		if (v == "true")
		{
			ret = true;
		}
		else if (v == "false")
		{
			ret = false;
		}
	}
	else
	{
		std::vector<std::string> elements = Split(v, ' ');
		switch (elements.size())
		{
		case 1:			
			ret = (float) atof(elements[0].c_str());
			break;
		case 2:			
			ret = vec2((float) atof(elements[0].c_str()), (float) atof(elements[1].c_str()));
			break;
		case 3:			
			ret = vec3((float) atof(elements[0].c_str()), (float) atof(elements[1].c_str()), (float) atof(elements[2].c_str()));
			break;
		case 4:			
			ret = vec4((float) atof(elements[0].c_str()), (float) atof(elements[1].c_str()), (float) atof(elements[2].c_str()), (float) atof(elements[3].c_str()));
			break;
		default:
			break;
		}
	}
	return ret;
}