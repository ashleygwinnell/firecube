#include <string>
#include <vector>
#include <map>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <SDL.h>
#include <windows.h>
#include "GLee.h"
#include "utils.h"	
#include "MyMath.h"	

using namespace FireCube;
#include <cmath>

vec4 FireCube::operator+(const vec4 &a,const vec4 &b)
{
	return vec4(a.x+b.x,a.y+b.y,a.z+b.z,a.w+b.w);
}
vec4 FireCube::operator-(const vec4 &a,const vec4 &b)
{
	return vec4(a.x-b.x,a.y-b.y,a.z-b.z,a.w-b.w);
}
vec4 FireCube::operator*(const vec4 &a,const float &b)
{
	return vec4(a.x*b,a.y*b,a.z*b,a.w*b);
}
vec4 FireCube::operator*(const float &a,const vec4 &b)
{
	return vec4(b.x*a,b.y*a,b.z*a,b.w*a);
}
vec4 FireCube::Cross(const vec4 a,const vec4 b)
{
	return vec4(a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x,a.w);
}
float FireCube::Dot(const vec4 a,const vec4 b)
{
	return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w;
}


void vec4::operator+=(const vec4 &src)
{	
	x+=src.x;
	y+=src.y;
	z+=src.z;
	w+=src.w;
}
void vec4::operator-=(vec4 &src)
{	
	x-=src.x;
	y-=src.y;
	z-=src.z;
	w-=src.w;
}
void vec4::operator*=(vec4 &src)
{	
	x*=src.x;
	y*=src.y;
	z*=src.z;
	w*=src.w;
}
void vec4::operator*=(float val)
{	
	x*=val;
	y*=val;
	z*=val;
	w*=val;
}
void vec4::operator/=(vec4 &src)
{
	x/=src.x;
	y/=src.y;
	z/=src.z;
	w/=src.w;
}

void vec4::operator/=(float val)
{
	x/=val;
	y/=val;
	z/=val;
	w/=val;
}
vec4 vec4::Normalize()
{
	const float m2 = Length2();
	if (m2 > 0.0f)
	{
		const float inv_mag = 1.0f/sqrt(m2);
		x = x * inv_mag;
		y = y * inv_mag;
		z = z * inv_mag;		
		w = w * inv_mag;
	}
	else
	{

		x = 1.0f; 
		y = z = w = 0.0f;		
	}
	return *this;
}
float vec4::Length() const
{
	float length;
	length=(float)sqrt(x*x+y*y+z*z+w*w);
	return length;
}
float vec4::Length2() const
{
	return x*x+y*y+z*z+w*w;
}
void vec4::Cross(const vec4 &p,const vec4 &q)
{
	x=p.y*q.z-p.z*q.y;
	y=p.z*q.x-p.x*q.z;
	z=p.x*q.y-p.y*q.x;
	w=p.w;
}
void vec4::SetLength(float l)
{
	float len=Length();

	x*=l/len;
	y*=l/len;
	z*=l/len;
	w*=l/len;
}
void vec4::RotateX(float ang)
{
	float cs=(float)cos(ang),sn=(float)sin(ang);
	float ny=y*cs-z*sn;
	float nz=y*sn+z*cs;
	y=ny;
	z=nz;
}
void vec4::RotateY(float ang)
{
	float cs=(float)cos(ang),sn=(float)sin(ang);
	float nx=x*cs-z*sn;
	float nz=x*sn+z*cs;
	x=nx;
	z=nz;
}
void vec4::RotateZ(float ang)
{
	float cs=(float)cos(ang),sn=(float)sin(ang);
	float nx=x*cs-y*sn;
	float ny=x*sn+y*cs;
	x=nx;
	y=ny;
}
vec4 vec4::operator*(mat4 &src)
{
	return vec4(x*src.m[0]+y*src.m[4]+z*src.m[8]+w*src.m[12],x*src.m[1]+y*src.m[5]+z*src.m[9]+w*src.m[13],x*src.m[2]+y*src.m[6]+z*src.m[10]+w*src.m[14],x*src.m[3]+y*src.m[7]+z*src.m[11]+w*src.m[15]);
}
void vec4::FromAngles(float angx,float angy)
{		
	float cosx;
	cosx=(float)cos(angx);

	x=cosx*(float)sin(-angy);
	y=(float)sin(angx);
	z=-cosx*(float)cos(angy);
}
inline bool vec4::Sensible() const
{
	if (!IsFinite(x))
		return false;
	if (!IsFinite(y))
		return false;
	if (!IsFinite(z))
		return false;
	if (!IsFinite(w))
		return false;

	return true;
}
vec2 vec4::ToVec2()
{
	return vec2(x,y);
}
vec3 vec4::ToVec3()
{
	return vec3(x,y,z);
}
float & vec4::operator[](unsigned int i)
{
	return *(&x+i);
}
const float & vec4::operator[](unsigned int i) const
{
	return *(&x+i);
}
float & vec4::operator()(unsigned int i) 
{
	return *(&x+i);
}
const float & vec4::operator()(unsigned int i) const
{
	return *(&x+i);
}