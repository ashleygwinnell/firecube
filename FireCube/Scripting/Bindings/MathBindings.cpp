#include "lua.hpp"
#include "LuaBridge.h"
#include "Scripting/LuaBindings.h"
#include "Math/Math.h"
#include "Math/Ray.h"

using namespace FireCube;
using namespace luabridge;

template <typename T>
T vec_mul(LuaRef a, LuaRef b)
{
	if (a.isUserdata())
	{
		T av = a.cast<T>();
		if (b.isUserdata())
		{
			T bv = b.cast<T>();
			return av * bv;
		}
		else if (b.isNumber())
		{
			return av * ((float)b);
		}
	}
	else if (a.isNumber())
	{
		T bv = b.cast<T>();
		return ((float)a) * bv;
	}

	return T(0.0f);
}

template <typename T>
struct MatHelper
{
	template <unsigned index>
	static float Get(T const* mat)
	{
		return mat->m[index];
	}

	template <unsigned index>
	static void Set(T* mat, float value)
	{
		mat->m[index] = value;
	}
};

void LuaBindings::InitMath(lua_State *luaState)
{
	getGlobalNamespace(luaState)
		.beginClass<vec3>("vec3")
			.addConstructor<void(*) (float, float, float)>()
			.addData("x", &vec3::x)
			.addData("y", &vec3::y)
			.addData("z", &vec3::z)
			.addFunction("Length", &vec3::Length)
			.addFunction("Length2", &vec3::Length2)
			.addFunction("Normalized", &vec3::Normalized)
			.addStaticFunction("Dot", (float(*)(const vec3 &, const vec3 &)) &FireCube::Dot)
			.addFunctionFree("__add", (vec3(*)(const vec3 &, const vec3 &)) &FireCube::operator+)
			.addFunctionFree("__sub", (vec3(*)(const vec3 &, const vec3 &)) &FireCube::operator-)
			.addFunctionFree("__mul", &vec_mul<vec3>)
			.addFunction("__unm", (vec3(vec3::*)()) &vec3::operator-)
			.addStaticFunction("Cross", (vec3(*)(const vec3 &, const vec3 &)) &FireCube::Cross)
		.endClass()
		.beginClass<vec2>("vec2")
			.addConstructor<void(*) (float, float)>()
			.addData("x", &vec2::x)
			.addData("y", &vec2::y)
			.addFunction("Length", &vec2::Length)
			.addFunction("Length2", &vec2::Length2)
			.addFunctionFree("__add", (vec2(*)(const vec2 &, const vec2 &)) &FireCube::operator+)
			.addFunctionFree("__sub", (vec2(*)(const vec2 &, const vec2 &)) &FireCube::operator-)
			.addFunctionFree("__mul", &vec_mul<vec2>)
			.addFunction("__unm", (vec2(vec2::*)()) &vec2::operator-)
			.addStaticFunction("Cross", (float(*)(const vec2 &, const vec2 &)) &FireCube::Cross)
		.endClass()
		.beginClass<Ray>("Ray")
			.addConstructor<void(*) (const vec3 &, const vec3 &)>()
			.addData("origin", &Ray::origin)
			.addData("direction", &Ray::direction)
		.endClass()
		.beginClass<Plane>("Plane")
		.addConstructor<void(*) (const vec3 &, float)>()
			.addProperty("normal", &Plane::GetNormal)
			.addProperty("distance", &Plane::GetDistance)
		.endClass()
		.beginClass<BoundingBox>("BoundingBox")
			.addConstructor<void(*) ()>()
			.addProperty("min", &BoundingBox::GetMin, &BoundingBox::SetMin)
			.addProperty("max", &BoundingBox::GetMax, &BoundingBox::SetMax)
			.addFunction("Expand", (void(BoundingBox::*)(const vec3 &)) &BoundingBox::Expand)
			.addProperty("width", &BoundingBox::GetWidth)
			.addProperty("height", &BoundingBox::GetHeight)
			.addProperty("depth", &BoundingBox::GetDepth)
			.addProperty("center", &BoundingBox::GetCenter)
		.endClass()
		.beginClass<mat4>("mat4")
			.addConstructor<void(*) ()>()
			.addFunction("RotateX", &mat4::RotateX)
			.addFunction("RotateY", &mat4::RotateY)
			.addFunction("RotateZ", &mat4::RotateZ)
			.addFunction("GetDirection", &mat4::GetDirection)
			.addFunction("Identity", &mat4::Identity)
			.addFunction("Translate", (void(mat4::*)(const vec3 &)) &mat4::Translate)
			.addProperty("m00", &MatHelper<mat4>::Get<0>, &MatHelper<mat4>::Set<0>)
			.addProperty("m10", &MatHelper<mat4>::Get<1>, &MatHelper<mat4>::Set<1>)
			.addProperty("m20", &MatHelper<mat4>::Get<2>, &MatHelper<mat4>::Set<2>)
			.addProperty("m30", &MatHelper<mat4>::Get<3>, &MatHelper<mat4>::Set<3>)
			.addProperty("m01", &MatHelper<mat4>::Get<4>, &MatHelper<mat4>::Set<4>)
			.addProperty("m11", &MatHelper<mat4>::Get<5>, &MatHelper<mat4>::Set<5>)
			.addProperty("m21", &MatHelper<mat4>::Get<6>, &MatHelper<mat4>::Set<6>)
			.addProperty("m31", &MatHelper<mat4>::Get<7>, &MatHelper<mat4>::Set<7>)
			.addProperty("m02", &MatHelper<mat4>::Get<8>, &MatHelper<mat4>::Set<8>)
			.addProperty("m12", &MatHelper<mat4>::Get<9>, &MatHelper<mat4>::Set<9>)
			.addProperty("m22", &MatHelper<mat4>::Get<10>, &MatHelper<mat4>::Set<10>)
			.addProperty("m32", &MatHelper<mat4>::Get<11>, &MatHelper<mat4>::Set<11>)
			.addProperty("m03", &MatHelper<mat4>::Get<12>, &MatHelper<mat4>::Set<12>)
			.addProperty("m13", &MatHelper<mat4>::Get<13>, &MatHelper<mat4>::Set<13>)
			.addProperty("m23", &MatHelper<mat4>::Get<14>, &MatHelper<mat4>::Set<14>)
			.addProperty("m33", &MatHelper<mat4>::Get<15>, &MatHelper<mat4>::Set<15>)
		.endClass();
}