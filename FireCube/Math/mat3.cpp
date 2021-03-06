#include "Math/Math.h"

using namespace FireCube;
#include <cmath>

const mat3 mat3::IDENTITY(1.0f, 0.0f, 0.0f, 
						  0.0f, 1.0f, 0.0f, 
						  0.0f, 0.0f, 1.0f);

mat3 FireCube::operator*(const mat3 &a, float b)
{
	mat3 ret;
	ret.m[0] = a.m[0] * b;
	ret.m[1] = a.m[1] * b;
	ret.m[2] = a.m[2] * b;
	ret.m[3] = a.m[3] * b;
	ret.m[4] = a.m[4] * b;
	ret.m[5] = a.m[5] * b;
	ret.m[6] = a.m[6] * b;
	ret.m[7] = a.m[7] * b;
	ret.m[8] = a.m[8] * b;
	return ret;
}

mat3 FireCube::operator*(float a, const mat3 &b)
{
	mat3 ret;
	ret.m[0] = b.m[0] * a;
	ret.m[1] = b.m[1] * a;
	ret.m[2] = b.m[2] * a;
	ret.m[3] = b.m[3] * a;
	ret.m[4] = b.m[4] * a;
	ret.m[5] = b.m[5] * a;
	ret.m[6] = b.m[6] * a;
	ret.m[7] = b.m[7] * a;
	ret.m[8] = b.m[8] * a;
	return ret;

}

vec3 FireCube::operator*(const mat3 &a, const vec3 &b)
{
	return vec3(b.x * a.m[0] + b.y * a.m[3] + b.z * a.m[6], b.x * a.m[1] + b.y * a.m[4] + b.z * a.m[7], b.x * a.m[2] + b.y * a.m[5] + b.z * a.m[8]);
}

vec3 FireCube::operator*(const vec3 &a, const mat3 &b)
{
	return vec3(a.x * b.m[0] + a.y * b.m[3] + a.z * b.m[6], a.x * b.m[1] + a.y * b.m[4] + a.z * b.m[7], a.x * b.m[2] + a.y * b.m[5] + a.z * b.m[8]);
}

inline mat3::mat3(float v11, float v21, float v31, // first column
				  float v12, float v22, float v32, // 2nd column
				  float v13, float v23, float v33  )
{
	m[0] = v11;
	m[1] = v21;
	m[2] = v31;

	m[3] = v12;
	m[4] = v22;
	m[5] = v32;

	m[6] = v13;
	m[7] = v23;
	m[8] = v33;
}

inline mat3::mat3(const vec3 & v1, // first column
				  const vec3 & v2,
				  const vec3 & v3)
{
	m[0] = v1.x;
	m[1] = v1.y;
	m[2] = v1.z;

	m[3] = v2.x;
	m[4] = v2.y;
	m[5] = v2.z;

	m[6] = v3.x;
	m[7] = v3.y;
	m[8] = v3.z;
}

inline void mat3::Identity()
{
	m[0] = m[4] = m[8] = 1;
	m[1] = m[2] = m[3] = m[5] = m[6] = m[7] = 0;
}

inline void mat3::Zero()
{
	m[0] = m[1] = m[2] = m[3] = m[4] = m[5] = m[6] = m[7] = m[8] = 0;
}

inline mat3 mat3::operator=(const mat3 &src)
{
	memcpy(m, src.m, sizeof(float) * 9);
	return (*this);
}

inline mat3 mat3::operator+(const mat3 &src) const
{
	mat3 ret;
	ret.m[0] = m[0] + src.m[0];
	ret.m[1] = m[1] + src.m[1];
	ret.m[2] = m[2] + src.m[2];
	ret.m[3] = m[3] + src.m[3];
	ret.m[4] = m[4] + src.m[4];
	ret.m[5] = m[5] + src.m[5];
	ret.m[6] = m[6] + src.m[6];
	ret.m[7] = m[7] + src.m[7];
	ret.m[8] = m[8] + src.m[8];
	return ret;
}

inline mat3 mat3::operator-(const mat3 &src) const
{
	mat3 ret;
	ret.m[0] = m[0] - src.m[0];
	ret.m[1] = m[1] - src.m[1];
	ret.m[2] = m[2] - src.m[2];
	ret.m[3] = m[3] - src.m[3];
	ret.m[4] = m[4] - src.m[4];
	ret.m[5] = m[5] - src.m[5];
	ret.m[6] = m[6] - src.m[6];
	ret.m[7] = m[7] - src.m[7];
	ret.m[8] = m[8] - src.m[8];
	return ret;
}

inline void mat3::operator+=(const mat3 &src)
{
	m[0] += src.m[0];
	m[1] += src.m[1];
	m[2] += src.m[2];
	m[3] += src.m[3];
	m[4] += src.m[4];
	m[5] += src.m[5];
	m[6] += src.m[6];
	m[7] += src.m[7];
	m[8] += src.m[8];
}

inline void mat3::operator-=(const mat3 &src)
{
	m[0] -= src.m[0];
	m[1] -= src.m[1];
	m[2] -= src.m[2];
	m[3] -= src.m[3];
	m[4] -= src.m[4];
	m[5] -= src.m[5];
	m[6] -= src.m[6];
	m[7] -= src.m[7];
	m[8] -= src.m[8];
}

mat3 mat3::operator*(const mat3 &src) const
{
	mat3 ret;
	ret.m[0] = m[0] * src.m[0] + m[3] * src.m[1] + m[6] * src.m[2];
	ret.m[1] = m[1] * src.m[0] + m[4] * src.m[1] + m[7] * src.m[2];
	ret.m[2] = m[2] * src.m[0] + m[5] * src.m[1] + m[8] * src.m[2];

	ret.m[3] = m[0] * src.m[3] + m[3] * src.m[4] + m[6] * src.m[5];
	ret.m[4] = m[1] * src.m[3] + m[4] * src.m[4] + m[7] * src.m[5];
	ret.m[5] = m[2] * src.m[3] + m[5] * src.m[4] + m[8] * src.m[5];

	ret.m[6] = m[0] * src.m[6] + m[3] * src.m[7] + m[6] * src.m[8];
	ret.m[7] = m[1] * src.m[6] + m[4] * src.m[7] + m[7] * src.m[8];
	ret.m[8] = m[2] * src.m[6] + m[5] * src.m[7] + m[8] * src.m[8];

	return ret;
}

void mat3::operator*=(const mat3 &src)
{
	mat3 tmp = *this;
	m[0] = tmp.m[0] * src.m[0] + tmp.m[3] * src.m[1] + tmp.m[6] * src.m[2];
	m[1] = tmp.m[1] * src.m[0] + tmp.m[4] * src.m[1] + tmp.m[7] * src.m[2];
	m[2] = tmp.m[2] * src.m[0] + tmp.m[5] * src.m[1] + tmp.m[8] * src.m[2];

	m[3] = tmp.m[0] * src.m[3] + tmp.m[3] * src.m[4] + tmp.m[6] * src.m[5];
	m[4] = tmp.m[1] * src.m[3] + tmp.m[4] * src.m[4] + tmp.m[7] * src.m[5];
	m[5] = tmp.m[2] * src.m[3] + tmp.m[5] * src.m[4] + tmp.m[8] * src.m[5];

	m[6] = tmp.m[0] * src.m[6] + tmp.m[3] * src.m[7] + tmp.m[6] * src.m[8];
	m[7] = tmp.m[1] * src.m[6] + tmp.m[4] * src.m[7] + tmp.m[7] * src.m[8];
	m[8] = tmp.m[2] * src.m[6] + tmp.m[5] * src.m[7] + tmp.m[8] * src.m[8];
}

void mat3::Scale(float x, float y, float z)
{
	mat3 scale = mat3::IDENTITY;
	scale.m[0] = x;
	scale.m[4] = y;
	scale.m[8] = z;
	(*this) = (*this) * scale;
}

void mat3::RotateX(float ang)
{
	mat3 rot = mat3::IDENTITY;
	float cs = (float)cos(ang);
	float sn = (float)sin(ang);

	rot.m[4] = cs;
	rot.m[5] = sn;
	rot.m[7] = -sn;
	rot.m[8] = cs;
	(*this) = (*this) * rot;

}

void mat3::RotateY(float ang)
{
	mat3 rot = mat3::IDENTITY;
	float cs = (float)cos(ang);
	float sn = (float)sin(ang);

	rot.m[0] = cs;
	rot.m[2] = -sn;
	rot.m[6] = sn;
	rot.m[8] = cs;
	(*this) = (*this) * rot;
}

void mat3::RotateZ(float ang)
{
	mat3 rot = mat3::IDENTITY;
	float cs = (float)cos(ang);
	float sn = (float)sin(ang);

	rot.m[0] = cs;
	rot.m[1] = sn;
	rot.m[3] = -sn;
	rot.m[4] = cs;
	(*this) = (*this) * rot;
}

void mat3::Transpose()
{
	mat3 t = *this;
	m[1] = t.m[3];
	m[2] = t.m[6];

	m[3] = t.m[1];
	m[5] = t.m[7];

	m[6] = t.m[2];
	m[7] = t.m[5];
}

void mat3::Inverse()
{
	mat3 A = *this;	
	float determinant =   A(0, 0) * (A(1, 1) * A(2, 2) - A(2, 1) * A(1, 2))
						- A(0, 1) * (A(1, 0) * A(2, 2) - A(1, 2) * A(2, 0))
						+ A(0, 2) * (A(1, 0) * A(2, 1) - A(1, 1) * A(2, 0));
	
	if (determinant == 0.0f)
	{
		return;
	}

	float invdet = 1.0f / determinant;
	
	m[0] =  (A(1, 1) * A(2, 2) - A(2, 1) * A(1, 2)) * invdet;
	m[3] = -(A(0, 1) * A(2, 2) - A(0, 2) * A(2, 1)) * invdet;
	m[6] =  (A(0, 1) * A(1, 2) - A(0, 2) * A(1, 1)) * invdet;
	m[1] = -(A(1, 0) * A(2, 2) - A(1, 2) * A(2, 0)) * invdet;
	m[4] =  (A(0, 0) * A(2, 2) - A(0, 2) * A(2, 0)) * invdet;
	m[7] = -(A(0, 0) * A(1, 2) - A(1, 0) * A(0, 2)) * invdet;
	m[2] =  (A(1, 0) * A(2, 1) - A(2, 0) * A(1, 1)) * invdet;
	m[5] = -(A(0, 0) * A(2, 1) - A(2, 0) * A(0, 1)) * invdet;
	m[8] =  (A(0, 0) * A(1, 1) - A(1, 0) * A(0, 1)) * invdet;	
}

vec3 mat3::GetDirection() const
{
	return vec3(-m[6], -m[7], -m[8]);	 
}

mat4 mat3::ToMat4() const
{
	mat4 ret = mat4::IDENTITY;

	ret.m[0] = m[0];
	ret.m[1] = m[1];
	ret.m[2] = m[2];

	ret.m[4] = m[3];
	ret.m[5] = m[4];
	ret.m[6] = m[5];

	ret.m[8] = m[6];
	ret.m[9] = m[7];
	ret.m[10] = m[8];

	return ret;
}

static inline vec3 FireCube::proj(const vec3 & v1, const vec3 & v2)
{
	return v1.Dot(v2) * v1 / v1.Length2();
}

inline void mat3::Orthonormalize()
{
	// Gram�Schmidt orthonormalization process.
	vec3 u1(operator()(0, 0), operator()(1, 0), operator()(2, 0));
	vec3 u2(operator()(0, 1), operator()(1, 1), operator()(2, 1));
	vec3 u3(operator()(0, 2), operator()(1, 2), operator()(2, 2));

	vec3 w1 = u1;
	w1.Normalize();


	vec3 w2 = (u2 - proj(w1, u2));
	w2.Normalize();
	vec3 w3 = (u3 - proj(w1, u3) - proj(w2, u3));
	w3.Normalize();

	m[0] = w1.x;
	m[1] = w1.y;
	m[2] = w1.z;

	m[3] = w2.x;
	m[4] = w2.y;
	m[5] = w2.z;

	m[6] = w3.x;
	m[7] = w3.y;
	m[8] = w3.z;

	if (Sensible() == false)
	{

		this->Identity();
	}
}

inline bool mat3::Sensible() const
{
	for (unsigned i = 0 ; i < 9 ; ++i)
	{
		if (!IsFinite(m[i]))
			return false;
	}
	return true;
}

inline vec3 mat3::GetCol(unsigned int i) const
{
	const unsigned int o = i * 3;
	return vec3(m[o], m[o + 1], m[o + 2]);
}

inline void mat3::SetCol(unsigned int i, const vec3 & col)
{
	const unsigned int o = i * 3;
	m[o]   = col[0];
	m[o + 1] = col[1];
	m[o + 2] = col[2];
}

vec3 mat3::ExtractEulerAngles() const
{
	vec3 rotation;
	rotation.y = asin(m[6]);
	if (std::abs(m[6]) < 0.999f)
	{
		rotation.x = atan2(-m[7], m[8]);
		rotation.z = atan2(-m[3], m[0]);
	}
	else
	{
		rotation.x = 0.0f;
		rotation.z = atan2(m[1], m[4]);
		
	}	
	return rotation;
}

void mat3::FromEulerAngles(vec3 euler)
{
	float x = euler.x, y = euler.y, z = euler.z;
	float a = cos(x), b = sin(x);
	float c = cos(y), d = sin(y);
	float e = cos(z), f = sin(z);

	float ae = a * e, af = a * f, be = b * e, bf = b * f;

	m[0] = c * e;
	m[3] = -c * f;
	m[6] = d;

	m[1] = af + be * d;
	m[4] = ae - bf * d;
	m[7] = -b * c;

	m[2] = bf - ae * d;
	m[5] = be + af * d;
	m[8] = a * c;
}
