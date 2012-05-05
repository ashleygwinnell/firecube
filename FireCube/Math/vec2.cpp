#include <string>
#include <vector>
#include <map>
#include <queue>
using namespace std;

#include "Utils/utils.h"
#include "Math/MyMath.h"

using namespace FireCube;
#include <cmath>

vec2 FireCube::operator+(const vec2 &a, const vec2 &b)
{
	return vec2(a.x + b.x, a.y + b.y);
}

vec2 FireCube::operator-(const vec2 &a, const vec2 &b)
{
	return vec2(a.x - b.x, a.y - b.y);
}

vec2 FireCube::operator*(const vec2 &a, float b)
{
	return vec2(a.x * b, a.y * b);
}

vec2 FireCube::operator*(float a, const vec2 &b)
{
	return vec2(b.x * a, b.y * a);
}

vec2 FireCube::operator/(const vec2 &a, float b)
{
	return vec2(a.x / b, a.y / b);
}

vec2 FireCube::operator/(float a, const vec2 &b)
{
	return vec2(b.x / a, b.y / a);
}

float FireCube::Cross(const vec2 &a, const vec2 &b)
{
	return (a.x * b.y - a.y * b.x);
}

float FireCube::Dot(const vec2 &a, const vec2 &b)
{
	return a.x * b.x + a.y * b.y;
}

void vec2::operator+=(const vec2 &src)
{
	x += src.x;
	y += src.y;
}

void vec2::operator-=(const vec2 &src)
{
	x -= src.x;
	y -= src.y;
}

void vec2::operator*=(const vec2 &src)
{
	x *= src.x;
	y *= src.y;
}

void vec2::operator*=(float val)
{
	x *= val;
	y *= val;
}

void vec2::operator/=(const vec2 &src)
{
	x /= src.x;
	y /= src.y;
}

void vec2::operator/=(float val)
{
	x /= val;
	y /= val;
}

vec2 vec2::Normalize()
{
	const float m2 = Length2();
	if (m2 > 0.0f)
	{
		const float inv_mag = 1.0f / sqrt(m2);
		x = x * inv_mag;
		y = y * inv_mag;
	}
	else
	{
		x = 1.0f;
		y = 0.0f;
	}
	return *this;
}

float vec2::Length() const
{
	return (float) sqrt(x * x + y * y);
}

float vec2::Length2() const
{
	return x * x + y * y;
}

float vec2::Cross(const vec2 &p, const vec2 &q) const
{
	return FireCube::Cross(p, q);
}

void vec2::SetLength(float l)
{
	float len = Length();

	x *= l / len;
	y *= l / len;
}

void vec2::Rotate(float ang)
{
	float cs = (float)cos(ang), sn = (float)sin(ang);
	float nx = x * cs - y * sn;
	float ny = x * sn + y * cs;
	x = nx;
	y = ny;
}

vec2 vec2::operator*(const mat4 &src) const
{
	return vec2(x * src.m[0] + y * src.m[4] + src.m[12], x * src.m[1] + y * src.m[5] + src.m[13]);
}

void vec2::FromAngle(float ang)
{
	x = (float)cos(ang);
	y = (float)sin(ang);
}

vec2 vec2::TransformCoordinate(const mat4 &m) const
{	
	float w = x * m.m[3] + y * m.m[7] + m.m[15];
	vec2 ret = vec2(x * m.m[0] + y * m.m[4] + m.m[12], x * m.m[1] + y * m.m[5] + m.m[13]);
	ret /= w;
	return ret;
}

vec2 vec2::TransformNormal(const mat4 &m) const
{
	mat4 t = m;
	vec2 ret = vec2(x * t.m[0] + y * t.m[4], x * t.m[1] + y * t.m[5]);
	return ret;
}

inline bool vec2::Sensible() const
{
	if (!IsFinite(x))
		return false;
	if (!IsFinite(y))
		return false;

	return true;
}

vec3 vec2::ToVec3() const
{
	return vec3(x, y, 0);
}

vec4 vec2::ToVec4() const
{
	return vec4(x, y, 0, 0);
}

float & vec2::operator[](unsigned int i)
{
	return *(&x + i);
}

const float & vec2::operator[](unsigned int i) const
{
	return *(&x + i);
}

float & vec2::operator()(unsigned int i)
{
	return *(&x + i);
}

const float & vec2::operator()(unsigned int i) const
{
	return *(&x + i);
}
