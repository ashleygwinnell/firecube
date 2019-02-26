#include "lua.hpp"
#include "sol.hpp"
#include "Scripting/LuaBindings.h"
#include "Math/Math.h"
#include "Math/Ray.h"
#include "Math/Frustum.h"

using namespace FireCube;

template <typename T>
struct MatHelper
{
	template <unsigned index>
	static float Get(const T &mat)
	{
		return mat.m[index];
	}

	template <unsigned index>
	static void Set(T &mat, float value)
	{
		mat.m[index] = value;
	}
};

void LuaBindings::InitMath(sol::state &luaState)
{	
	luaState.new_usertype<vec2>("vec2",
		sol::constructors<vec2(), vec2(float), vec2(float, float)>(),
		"x", &vec2::x,
		"y", &vec2::y,
		"Length", &vec2::Length,
		"Length2", &vec2::Length2,		
		sol::meta_function::addition, sol::resolve<vec2(const vec2&, const vec2&)>(::operator+),
		sol::meta_function::subtraction, sol::resolve<vec2(const vec2&, const vec2&)>(::operator-),
		sol::meta_function::multiplication, sol::overload(sol::resolve<vec2(const vec2&, float)>(::operator*),
			sol::resolve<vec2(const vec2&) const>(&vec2::operator*)),
		sol::meta_function::unary_minus, sol::resolve<vec2()>(&vec2::operator-),
		"Cross", sol::resolve<float(const vec2 &, const vec2 &)>(&FireCube::Cross));

	luaState.new_usertype<vec3>("vec3",
		sol::constructors<vec3(), vec3(float), vec3(float, float, float)>(),
		"x", &vec3::x,
		"y", &vec3::y, 
		"z", &vec3::z,
		"Length", &vec3::Length,		
		"Length2", &vec3::Length2,
		"Normalized", &vec3::Normalized,
		"Normalize", &vec3::Normalize,
		"TransformNormal", &vec3::TransformNormal,
		"Dot", sol::resolve<float(const vec3&, const vec3&)>(&FireCube::Dot),
		sol::meta_function::addition, sol::resolve<vec3(const vec3&, const vec3&)>(::operator+),
		sol::meta_function::subtraction, sol::resolve<vec3(const vec3&, const vec3&)>(::operator-),		
		sol::meta_function::multiplication, sol::overload(sol::resolve<vec3(const vec3&, float)>(::operator*),
			sol::resolve<vec3(float, const vec3&)>(::operator*),
			sol::resolve<vec3(const vec3&) const>(&vec3::operator*)),
		sol::meta_function::unary_minus, sol::resolve<vec3()>(&vec3::operator-),
		"Cross", sol::resolve<vec3(const vec3 &, const vec3 &)>(&FireCube::Cross));

	luaState.new_usertype<vec4>("vec4",
		sol::constructors<vec4(), vec4(float), vec4(float, float, float, float)>(),
		"x", &vec4::x,
		"y", &vec4::y,
		"z", &vec4::z,
		"w", &vec4::w,
		"Length", &vec4::Length,
		"Length2", &vec4::Length2,
		"Normalized", &vec4::Normalized,		
		"Dot", sol::resolve<float(const vec4&, const vec4&)>(&FireCube::Dot),
		sol::meta_function::addition, sol::resolve<vec4(const vec4&, const vec4&)>(::operator+),
		sol::meta_function::subtraction, sol::resolve<vec4(const vec4&, const vec4&)>(::operator-),
		sol::meta_function::multiplication, sol::overload(sol::resolve<vec4(const vec4&, float)>(::operator*),
			sol::resolve<vec4(const vec4&) const>(&vec4::operator*)),
		sol::meta_function::unary_minus, sol::resolve<vec4()>(&vec4::operator-),
		"Cross", sol::resolve<vec4(const vec4 &, const vec4 &)>(&FireCube::Cross));

	luaState.new_usertype<Ray>("Ray",
		sol::constructors<Ray(const vec3 &, const vec3 &)>(),
		"origin", &Ray::origin,
		"direction", &Ray::direction);

	luaState.new_usertype<Plane>("Plane",
		sol::constructors<Plane(const vec3 &, float)>(),
		"normal", sol::property(&Plane::GetNormal),
		"distance", sol::property(sol::resolve<float() const>(&Plane::GetDistance)),
		"GetDistance", sol::resolve<float(const vec3 &point) const>(&Plane::GetDistance));

	luaState.new_usertype<BoundingBox>("BoundingBox",
		sol::constructors<BoundingBox()>(),
		"min", sol::property(&BoundingBox::GetMin, &BoundingBox::SetMin),
		"max", sol::property(&BoundingBox::GetMax, &BoundingBox::SetMax),
		"Expand", sol::resolve<void(const vec3 &)>(&BoundingBox::Expand),
		"width", sol::property(&BoundingBox::GetWidth),
		"height", sol::property(&BoundingBox::GetHeight),
		"depth", sol::property(&BoundingBox::GetDepth),
		"center", sol::property(&BoundingBox::GetCenter));

	luaState.new_usertype<mat4>("mat4",
		sol::constructors<mat4()>(),
		"RotateX", &mat4::RotateX,
		"RotateY", &mat4::RotateY,
		"RotateZ", &mat4::RotateZ,
		"GetDirection", &mat4::GetDirection,
		"Identity", &mat4::Identity,
		"Translate", sol::resolve<void(const vec3 &)>(&mat4::Translate),		
		"Inverse", &mat4::Inverse,
		sol::meta_function::multiplication, sol::resolve<vec3(const vec3&) const>(&mat4::operator*),
		"m00", sol::property(&MatHelper<mat4>::Get<0>, &MatHelper<mat4>::Set<0>),
		"m10", sol::property(&MatHelper<mat4>::Get<1>, &MatHelper<mat4>::Set<1>),
		"m20", sol::property(&MatHelper<mat4>::Get<2>, &MatHelper<mat4>::Set<2>),
		"m30", sol::property(&MatHelper<mat4>::Get<3>, &MatHelper<mat4>::Set<3>),
		"m01", sol::property(&MatHelper<mat4>::Get<4>, &MatHelper<mat4>::Set<4>),
		"m11", sol::property(&MatHelper<mat4>::Get<5>, &MatHelper<mat4>::Set<5>),
		"m21", sol::property(&MatHelper<mat4>::Get<6>, &MatHelper<mat4>::Set<6>),
		"m31", sol::property(&MatHelper<mat4>::Get<7>, &MatHelper<mat4>::Set<7>),
		"m02", sol::property(&MatHelper<mat4>::Get<8>, &MatHelper<mat4>::Set<8>),
		"m12", sol::property(&MatHelper<mat4>::Get<9>, &MatHelper<mat4>::Set<9>),
		"m22", sol::property(&MatHelper<mat4>::Get<10>, &MatHelper<mat4>::Set<10>),
		"m32", sol::property(&MatHelper<mat4>::Get<11>, &MatHelper<mat4>::Set<11>),
		"m03", sol::property(&MatHelper<mat4>::Get<12>, &MatHelper<mat4>::Set<12>),
		"m13", sol::property(&MatHelper<mat4>::Get<13>, &MatHelper<mat4>::Set<13>),
		"m23", sol::property(&MatHelper<mat4>::Get<14>, &MatHelper<mat4>::Set<14>),
		"m33", sol::property(&MatHelper<mat4>::Get<15>, &MatHelper<mat4>::Set<15>));

	luaState.new_usertype<quat>("quat",
		sol::constructors<quat()>(),
		"x", &quat::x,
		"y", &quat::y,
		"z", &quat::z,
		"w", &quat::w,
		"FromEulerAngles", [](float x, float y, float z) { return quat(x, y, z); },
		"FromAxisAngle", [](vec3 axis, float angle) { return quat(axis, angle); },
		sol::meta_function::multiplication, sol::resolve<quat(const quat &, const quat &)>(::operator*));

	luaState.new_usertype<Frustum>("Frustum",
		sol::constructors<Frustum()>(),
		"leftPlane", sol::property(&Frustum::GetLeftPlane),
		"rightPlane", sol::property(&Frustum::GetRightPlane),
		"topPlane", sol::property(&Frustum::GetTopPlane),
		"bottomPlane", sol::property(&Frustum::GetBottomPlane),
		"farPlane", sol::property(&Frustum::GetFarPlane),
		"nearPlane", sol::property(&Frustum::GetNearPlane));
}