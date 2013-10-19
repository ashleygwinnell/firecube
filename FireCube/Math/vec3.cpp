#include "Math/Math.h"

using namespace FireCube;
#include <cmath>

const vec3 vec3::ZERO(0.0f);

vec3 FireCube::operator+(const vec3 &a, const vec3 &b)
{
	return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

vec3 FireCube::operator-(const vec3 &a, const vec3 &b)
{
	return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

vec3 FireCube::operator*(const vec3 &a, float b)
{
	return vec3(a.x * b, a.y * b, a.z * b);
}

vec3 FireCube::operator*(float a, const vec3 &b)
{
	return vec3(b.x * a, b.y * a, b.z * a);
}

vec3 FireCube::operator/(const vec3 &a, float b)
{
	return vec3(a.x / b, a.y / b, a.z / b);
}

vec3 FireCube::operator/(float a, const vec3 &b)
{
	return vec3(b.x / a, b.y / a, b.z / a);
}

vec3 FireCube::Cross(const vec3 &a, const vec3 &b)
{
	return vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

float FireCube::Dot(const vec3 &a, const vec3 &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

void vec3::operator+=(const vec3 &src)
{
	x += src.x;
	y += src.y;
	z += src.z;
}

void vec3::operator-=(const vec3 &src)
{
	x -= src.x;
	y -= src.y;
	z -= src.z;
}

void vec3::operator*=(const vec3 &src)
{
	x *= src.x;
	y *= src.y;
	z *= src.z;
}

void vec3::operator*=(float val)
{
	x *= val;
	y *= val;
	z *= val;
}

void vec3::operator/=(const vec3 &src)
{
	x /= src.x;
	y /= src.y;
	z /= src.z;
}

void vec3::operator/=(float val)
{
	x /= val;
	y /= val;
	z /= val;
}

void vec3::Normalize()
{
	const float m2 = Length2();
	if (m2 > 0.0f)
	{
		const float inv_mag = 1.0f / sqrt(m2);
		x = x * inv_mag;
		y = y * inv_mag;
		z = z * inv_mag;
	}
	else
	{
		x = 1.0f;
		y = z = 0.0f;
	}	
}

vec3 vec3::Normalized() const
{
	vec3 ret = *this;
	ret.Normalize();
	return ret;
}

float vec3::Length() const
{
	return (float) sqrt(x * x + y * y + z * z);
}

float vec3::Length2() const
{
	return x * x + y * y + z * z;
}

void vec3::Cross(const vec3 &p, const vec3 &q)
{
	x = p.y * q.z - p.z * q.y;
	y = p.z * q.x - p.x * q.z;
	z = p.x * q.y - p.y * q.x;
}

void vec3::SetLength(float l)
{
	float len = Length();

	x *= l / len;
	y *= l / len;
	z *= l / len;
}

void vec3::RotateX(float ang)
{
	float cs = (float)cos(ang), sn = (float)sin(ang);
	float ny = y * cs - z * sn;
	float nz = y * sn + z * cs;
	y = ny;
	z = nz;
}

void vec3::RotateY(float ang)
{
	float cs = (float)cos(ang), sn = (float)sin(ang);
	float nx = x * cs - z * sn;
	float nz = x * sn + z * cs;
	x = nx;
	z = nz;
}

void vec3::RotateZ(float ang)
{
	float cs = (float)cos(ang), sn = (float)sin(ang);
	float nx = x * cs - y * sn;
	float ny = x * sn + y * cs;
	x = nx;
	y = ny;
}

vec3 vec3::operator*(const mat4 &src) const
{
	return vec3(x * src.m[0] + y * src.m[4] + z * src.m[8] + src.m[12], x * src.m[1] + y * src.m[5] + z * src.m[9] + src.m[13], x * src.m[2] + y * src.m[6] + z * src.m[10] + src.m[14]);
}

void vec3::FromAngles(float angx, float angy)
{
	float cosx;
	cosx = (float)cos(angx);

	x = cosx * (float)sin(-angy);
	y = (float)sin(angx);
	z = -cosx * (float)cos(angy);
}

vec3 vec3::TransformCoordinate(const mat4 &m) const
{
	float w = x * m.m[3] + y * m.m[7] + z * m.m[11] + m.m[15];
	vec3 ret = vec3(x * m.m[0] + y * m.m[4] + z * m.m[8] + m.m[12], x * m.m[1] + y * m.m[5] + z * m.m[9] + m.m[13], x * m.m[2] + y * m.m[6] + z * m.m[10] + m.m[14]);
	ret /= w;
	return ret;
}

vec3 vec3::TransformNormal(const mat4 &m) const
{
	mat4 t = m;
	vec3 ret = vec3(x * t.m[0] + y * t.m[4] + z * t.m[8], x * t.m[1] + y * t.m[5] + z * t.m[9], x * t.m[2] + y * t.m[6] + z * m.m[10]);
	return ret;
}

inline bool vec3::Sensible() const
{
	if (!IsFinite(x))
		return false;
	if (!IsFinite(y))
		return false;
	if (!IsFinite(z))
		return false;

	return true;
}

vec2 vec3::ToVec2() const
{
	return vec2(x, y);
}

vec4 vec3::ToVec4() const
{
	return vec4(x, y, z, 0);
}

float & vec3::operator[](unsigned int i)
{
	return *(&x + i);
}

const float & vec3::operator[](unsigned int i) const
{
	return *(&x + i);
}

float & vec3::operator()(unsigned int i)
{
	return *(&x + i);
}

const float & vec3::operator()(unsigned int i) const
{
	return *(&x + i);
}