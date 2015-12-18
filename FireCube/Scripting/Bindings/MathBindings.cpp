#include "lua.hpp"
#include "LuaIntf.h"
#include "Scripting/LuaBindings.h"
#include "Math/Math.h"
#include "Math/Ray.h"

using namespace FireCube;
using namespace LuaIntf;

template <typename T>
T vec_mul(LuaRef a, LuaRef b)
{	 	
	if (a.type() == LuaTypeID::USERDATA)
	{
		T av = a.toValue<T>();
		if (b.type() == LuaTypeID::USERDATA)
		{
			T bv = b.toValue<T>();
			return av * bv;
		}
		else if (b.type() == LuaTypeID::NUMBER)
		{
			return av * b.toValue<float>();
		}		
	}
	else if (a.type() == LuaTypeID::NUMBER)
	{
		T bv = b.toValue<T>();
		return a.toValue<float>() * bv;
	}

	return T(0.0f);
}

template <typename T>
int vec_add(T *a, lua_State *L, const T &b)
{
	Lua::push(L, *a + b);
	return 1;
}

template <typename T>
int vec_sub(T *a, lua_State *L, const T &b)
{
	Lua::push(L, *a - b);
	return 1;
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

int QuatFromEulerAngles(lua_State *L, float x, float y, float z)
{
	Lua::push(L, quat(x, y, z));
	return 1;
}

int QuatFromAxisAngle(lua_State *L, vec3 axis, float angle)
{
	Lua::push(L, quat(axis, angle));
	return 1;
}

void LuaBindings::InitMath(lua_State *luaState)
{	
	LuaBinding(luaState)
		.beginClass<vec4>("vec4")
			.addConstructor(LUA_ARGS(_opt<float>, _opt<float>, _opt<float>, _opt<float>))
			.addVariable("x", &vec4::x)
			.addVariable("y", &vec4::y)
			.addVariable("z", &vec4::z)
			.addVariable("w", &vec4::w)
			.addFunction("Length", &vec4::Length)
			.addFunction("Length2", &vec4::Length2)
			.addFunction("Normalized", &vec4::Normalized)
			.addStaticFunction("Dot", (float(*)(const vec4 &, const vec4 &)) &FireCube::Dot)
			.addFunction("__add", &vec_add<vec4>)
			.addFunction("__sub", &vec_sub<vec4>)
			.addMetaFunction("__mul", &vec_mul<vec4>)
			.addFunction("__unm", (vec4(vec4::*)()) &vec4::operator-)
			.addStaticFunction("Cross", (vec4(*)(const vec4 &, const vec4 &)) &FireCube::Cross)
		.endClass()
		.beginClass<vec3>("vec3")
			.addConstructor(LUA_ARGS(_opt<float>, _opt<float>, _opt<float>))
			.addVariable("x", &vec3::x)
			.addVariable("y", &vec3::y)
			.addVariable("z", &vec3::z)
			.addFunction("Length", &vec3::Length)
			.addFunction("Length2", &vec3::Length2)
			.addFunction("Normalized", &vec3::Normalized)
			.addFunction("Normalize", &vec3::Normalize)
			.addStaticFunction("Dot", (float(*)(const vec3 &, const vec3 &)) &FireCube::Dot)
			.addFunction("__add", &vec_add<vec3>)
			.addFunction("__sub", &vec_sub<vec3>)
			.addMetaFunction("__mul", &vec_mul<vec3>)
			.addFunction("__unm", (vec3(vec3::*)()) &vec3::operator-)
			.addStaticFunction("Cross", (vec3(*)(const vec3 &, const vec3 &)) &FireCube::Cross)
		.endClass()
		.beginClass<vec2>("vec2")
			.addConstructor(LUA_ARGS(_opt<float>, _opt<float>))
			.addVariable("x", &vec2::x)
			.addVariable("y", &vec2::y)
			.addFunction("Length", &vec2::Length)
			.addFunction("Length2", &vec2::Length2)
			.addFunction("__add", &vec_add<vec2>)
			.addFunction("__sub", &vec_sub<vec2>)
			.addMetaFunction("__mul", &vec_mul<vec2>)
			.addFunction("__unm", (vec2(vec2::*)()) &vec2::operator-)
			.addStaticFunction("Cross", (float(*)(const vec2 &, const vec2 &)) &FireCube::Cross)
		.endClass()
		.beginClass<Ray>("Ray")
			.addConstructor(LUA_ARGS(const vec3 &, const vec3 &))
			.addVariable("origin", &Ray::origin)
			.addVariable("direction", &Ray::direction)
		.endClass()
		.beginClass<Plane>("Plane")
		.addConstructor(LUA_ARGS(const vec3 &, float))
			.addProperty("normal", &Plane::GetNormal)
			.addProperty("distance", (float(Plane::*)() const) &Plane::GetDistance)
		.endClass()
		.beginClass<BoundingBox>("BoundingBox")
			.addConstructor(LUA_ARGS())
			.addProperty("min", &BoundingBox::GetMin, &BoundingBox::SetMin)
			.addProperty("max", &BoundingBox::GetMax, &BoundingBox::SetMax)
			.addFunction("Expand", (void(BoundingBox::*)(const vec3 &)) &BoundingBox::Expand)
			.addProperty("width", &BoundingBox::GetWidth)
			.addProperty("height", &BoundingBox::GetHeight)
			.addProperty("depth", &BoundingBox::GetDepth)
			.addProperty("center", &BoundingBox::GetCenter)
		.endClass()
		.beginClass<mat4>("mat4")
			.addConstructor(LUA_ARGS())
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
		.endClass()
		.beginClass<quat>("quat")
			.addConstructor(LUA_ARGS())			
			.addVariable("x", &quat::x)
			.addVariable("y", &quat::y)
			.addVariable("z", &quat::z)
			.addVariable("w", &quat::w)
			.addStaticFunction("FromEulerAngles", &QuatFromEulerAngles)
			.addStaticFunction("FromAxisAngle", &QuatFromAxisAngle)
			.addMetaFunction("__mul", (quat(*)(const quat &, const quat &)) &operator*)
		.endClass();
}