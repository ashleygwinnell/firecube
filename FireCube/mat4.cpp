#include <string>
#include <vector>
#include <map>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <SDL/SDL.h>
#include <windows.h>
#include "GLee.h"
#include "FireCube.h"
using namespace FireCube;
#include <cmath>

inline void mat4::Identity()
{
	m[0]=m[5]=m[10]=m[15]=1;
	m[1]=m[2]=m[3]=m[4]=m[6]=m[7]=m[8]=m[9]=m[11]=m[12]=m[13]=m[14]=0;
}
inline void mat4::Zero()
{	
	m[0]=m[1]=m[2]=m[3]=m[4]=m[5]=m[6]=m[7]=m[8]=m[9]=m[10]=m[11]=m[12]=m[13]=m[14]=m[15]=0;
}
inline mat4 mat4::operator=(mat4 &src)
{
	memcpy(m,src.m,sizeof(float)*16);
	return (*this);
}
inline mat4 mat4::operator+(mat4 &src)
{
	mat4 ret;
	ret.m[0]=m[0]+src.m[0];
	ret.m[1]=m[1]+src.m[1];
	ret.m[2]=m[2]+src.m[2];
	ret.m[3]=m[3]+src.m[3];
	ret.m[4]=m[4]+src.m[4];
	ret.m[5]=m[5]+src.m[5];
	ret.m[6]=m[6]+src.m[6];
	ret.m[7]=m[7]+src.m[7];
	ret.m[8]=m[8]+src.m[8];
	ret.m[9]=m[9]+src.m[9];
	ret.m[10]=m[10]+src.m[10];
	ret.m[11]=m[11]+src.m[11];
	ret.m[12]=m[12]+src.m[12];
	ret.m[13]=m[13]+src.m[13];
	ret.m[14]=m[14]+src.m[14];
	ret.m[15]=m[15]+src.m[15];
	return ret;	
}
inline mat4 mat4::operator-(mat4 &src)
{
	mat4 ret;
	ret.m[0]=m[0]-src.m[0];
	ret.m[1]=m[1]-src.m[1];
	ret.m[2]=m[2]-src.m[2];
	ret.m[3]=m[3]-src.m[3];
	ret.m[4]=m[4]-src.m[4];
	ret.m[5]=m[5]-src.m[5];
	ret.m[6]=m[6]-src.m[6];
	ret.m[7]=m[7]-src.m[7];
	ret.m[8]=m[8]-src.m[8];
	ret.m[9]=m[9]-src.m[9];
	ret.m[10]=m[10]-src.m[10];
	ret.m[11]=m[11]-src.m[11];
	ret.m[12]=m[12]-src.m[12];
	ret.m[13]=m[13]-src.m[13];
	ret.m[14]=m[14]-src.m[14];
	ret.m[15]=m[15]-src.m[15];
	return ret;	
}
inline void mat4::operator+=(mat4 &src)
{
	m[0]+=src.m[0];	
	m[1]+=src.m[1];
	m[2]+=src.m[2];
	m[3]+=src.m[3];
	m[4]+=src.m[4];
	m[5]+=src.m[5];
	m[6]+=src.m[6];
	m[7]+=src.m[7];
	m[8]+=src.m[8];
	m[9]+=src.m[9];
	m[10]+=src.m[10];
	m[11]+=src.m[11];
	m[12]+=src.m[12];
	m[13]+=src.m[13];
	m[14]+=src.m[14];
	m[15]+=src.m[15];
}
inline void mat4::operator-=(mat4 &src)
{
	m[0]-=src.m[0];	
	m[1]-=src.m[1];
	m[2]-=src.m[2];
	m[3]-=src.m[3];
	m[4]-=src.m[4];
	m[5]-=src.m[5];
	m[6]-=src.m[6];
	m[7]-=src.m[7];
	m[8]-=src.m[8];
	m[9]-=src.m[9];
	m[10]-=src.m[10];
	m[11]-=src.m[11];
	m[12]-=src.m[12];
	m[13]-=src.m[13];
	m[14]-=src.m[14];
	m[15]-=src.m[15];
}
mat4 mat4::operator*(float src)
{
	mat4 ret;
	ret.m[0]=m[0]*src;
	ret.m[1]=m[1]*src;
	ret.m[2]=m[2]*src;
	ret.m[3]=m[3]*src;
	ret.m[4]=m[4]*src;
	ret.m[5]=m[5]*src;
	ret.m[6]=m[6]*src;
	ret.m[7]=m[7]*src;
	ret.m[8]=m[8]*src;
	ret.m[9]=m[9]*src;
	ret.m[10]=m[10]*src;
	ret.m[11]=m[11]*src;
	ret.m[12]=m[12]*src;
	ret.m[13]=m[13]*src;
	ret.m[14]=m[14]*src;
	ret.m[15]=m[15]*src;
	return ret;
}
vec3 mat4::operator*(vec3 &src)
{
	return vec3(src.x*m[0]+src.y*m[4]+src.z*m[8]+m[12],src.x*m[1]+src.y*m[5]+src.z*m[9]+m[13],src.x*m[2]+src.y*m[6]+src.z*m[10]+m[14]);
}
mat4 mat4::operator*(mat4 &src)
{
	mat4 ret;
	ret.m[0]=m[0]*src.m[0]+m[4]*src.m[1]+m[8]*src.m[2]+m[12]*src.m[3];
	ret.m[1]=m[1]*src.m[0]+m[5]*src.m[1]+m[9]*src.m[2]+m[13]*src.m[3];
	ret.m[2]=m[2]*src.m[0]+m[6]*src.m[1]+m[10]*src.m[2]+m[14]*src.m[3];
	ret.m[3]=m[3]*src.m[0]+m[7]*src.m[1]+m[11]*src.m[2]+m[15]*src.m[3];

	ret.m[4]=m[0]*src.m[4]+m[4]*src.m[5]+m[8]*src.m[6]+m[12]*src.m[7];
	ret.m[5]=m[1]*src.m[4]+m[5]*src.m[5]+m[9]*src.m[6]+m[13]*src.m[7];
	ret.m[6]=m[2]*src.m[4]+m[6]*src.m[5]+m[10]*src.m[6]+m[14]*src.m[7];
	ret.m[7]=m[3]*src.m[4]+m[7]*src.m[5]+m[11]*src.m[6]+m[15]*src.m[7];

	ret.m[8]=m[0]*src.m[8]+m[4]*src.m[9]+m[8]*src.m[10]+m[12]*src.m[11];
	ret.m[9]=m[1]*src.m[8]+m[5]*src.m[9]+m[9]*src.m[10]+m[13]*src.m[11];
	ret.m[10]=m[2]*src.m[8]+m[6]*src.m[9]+m[10]*src.m[10]+m[14]*src.m[11];
	ret.m[11]=m[3]*src.m[8]+m[7]*src.m[9]+m[11]*src.m[10]+m[15]*src.m[11];

	ret.m[12]=m[0]*src.m[12]+m[4]*src.m[13]+m[8]*src.m[14]+m[12]*src.m[15];
	ret.m[13]=m[1]*src.m[12]+m[5]*src.m[13]+m[9]*src.m[14]+m[13]*src.m[15];
	ret.m[14]=m[2]*src.m[12]+m[6]*src.m[13]+m[10]*src.m[14]+m[14]*src.m[15];
	ret.m[15]=m[3]*src.m[12]+m[7]*src.m[13]+m[11]*src.m[14]+m[15]*src.m[15];

	return ret;
}
void mat4::operator*=(mat4 &src)
{
	mat4 tmp=*this;	
	m[0]=tmp.m[0]*src.m[0]+tmp.m[4]*src.m[1]+tmp.m[8]*src.m[2]+tmp.m[12]*src.m[3];
	m[1]=tmp.m[1]*src.m[0]+tmp.m[5]*src.m[1]+tmp.m[9]*src.m[2]+tmp.m[13]*src.m[3];
	m[2]=tmp.m[2]*src.m[0]+tmp.m[6]*src.m[1]+tmp.m[10]*src.m[2]+tmp.m[14]*src.m[3];
	m[3]=tmp.m[3]*src.m[0]+tmp.m[7]*src.m[1]+tmp.m[11]*src.m[2]+tmp.m[15]*src.m[3];

	m[4]=tmp.m[0]*src.m[4]+tmp.m[4]*src.m[5]+tmp.m[8]*src.m[6]+tmp.m[12]*src.m[7];
	m[5]=tmp.m[1]*src.m[4]+tmp.m[5]*src.m[5]+tmp.m[9]*src.m[6]+tmp.m[13]*src.m[7];
	m[6]=tmp.m[2]*src.m[4]+tmp.m[6]*src.m[5]+tmp.m[10]*src.m[6]+tmp.m[14]*src.m[7];
	m[7]=tmp.m[3]*src.m[4]+tmp.m[7]*src.m[5]+tmp.m[11]*src.m[6]+tmp.m[15]*src.m[7];

	m[8]=tmp.m[0]*src.m[8]+tmp.m[4]*src.m[9]+tmp.m[8]*src.m[10]+tmp.m[12]*src.m[11];
	m[9]=tmp.m[1]*src.m[8]+tmp.m[5]*src.m[9]+tmp.m[9]*src.m[10]+tmp.m[13]*src.m[11];
	m[10]=tmp.m[2]*src.m[8]+tmp.m[6]*src.m[9]+tmp.m[10]*src.m[10]+tmp.m[14]*src.m[11];
	m[11]=tmp.m[3]*src.m[8]+tmp.m[7]*src.m[9]+tmp.m[11]*src.m[10]+tmp.m[15]*src.m[11];

	m[12]=tmp.m[0]*src.m[12]+tmp.m[4]*src.m[13]+tmp.m[8]*src.m[14]+tmp.m[12]*src.m[15];
	m[13]=tmp.m[1]*src.m[12]+tmp.m[5]*src.m[13]+tmp.m[9]*src.m[14]+tmp.m[13]*src.m[15];
	m[14]=tmp.m[2]*src.m[12]+tmp.m[6]*src.m[13]+tmp.m[10]*src.m[14]+tmp.m[14]*src.m[15];
	m[15]=tmp.m[3]*src.m[12]+tmp.m[7]*src.m[13]+tmp.m[11]*src.m[14]+tmp.m[15]*src.m[15];
}
void mat4::Translate(float x,float y,float z)
{
	mat4 trans;
	trans.m[12]=x;
	trans.m[13]=y;
	trans.m[14]=z;
	(*this)=(*this)*trans;
}
void mat4::Translate(vec3 t)
{
	Translate(t.x,t.y,t.z);
}
void mat4::Scale(float x,float y,float z)
{
	mat4 scale;
	scale.m[0]=x;
	scale.m[5]=y;
	scale.m[10]=z;
	(*this)=(*this)*scale;
}
void mat4::RotateX(float ang)
{
	mat4 rot;
	float cs=(float)cos(ang);
	float sn=(float)sin(ang);

	rot.m[5]=cs;
	rot.m[6]=-sn;
	rot.m[9]=sn;
	rot.m[10]=cs;
	(*this)=(*this)*rot;

}
void mat4::RotateY(float ang)
{
	mat4 rot;
	float cs=(float)cos(ang);
	float sn=(float)sin(ang);

	rot.m[0]=cs;
	rot.m[2]=sn;
	rot.m[8]=-sn;
	rot.m[10]=cs;
	(*this)=(*this)*rot;
}
void mat4::RotateZ(float ang)
{
	mat4 rot;
	float cs=(float)cos(ang);
	float sn=(float)sin(ang);

	rot.m[0]=cs;
	rot.m[1]=-sn;
	rot.m[4]=sn;
	rot.m[5]=cs;
	(*this)=(*this)*rot;
}
void mat4::Rotate(vec4 rot)
{
	mat4 rotm;
	float c = cos(rot.w);
	float s = sin(rot.w);
	float t = 1.0f - c;
	rotm.m[0]=c+rot.x*rot.x*t;
	rotm.m[5]=c+rot.y*rot.y*t;
	rotm.m[10]=c+rot.z*rot.z*t;
	
	float tmp1 = rot.x*rot.y*t;
	float tmp2 = rot.z*s;
	rotm.m[4] = tmp1 + tmp2;
	rotm.m[1] = tmp1 - tmp2;
	tmp1 = rot.x*rot.z*t;
	tmp2 = rot.y*s;
	rotm.m[8] = tmp1 - tmp2;
	rotm.m[2] = tmp1 + tmp2;
	tmp1 = rot.y*rot.z*t;
	tmp2 = rot.x*s;
	rotm.m[9] = tmp1 + tmp2;
	rotm.m[6] = tmp1 - tmp2;

	(*this)=(*this)*rotm;
}
void mat4::Transpose()
{
	mat4 t=*this;
	m[1]=t.m[4];
	m[2]=t.m[8];
	m[3]=t.m[12];

	m[4]=t.m[1];
	m[6]=t.m[9];
	m[7]=t.m[13];

	m[8]=t.m[2];
	m[9]=t.m[6];
	m[11]=t.m[14];

	m[12]=t.m[3];
	m[13]=t.m[7];
	m[14]=t.m[11];
}
void mat4::Inverse()
{
	mat4 &mm=*this;
	float d=mm(0,3)*mm(1,2)*mm(2,1)*mm(3,0) - mm(0,2)*mm(1,3)*mm(2,1)*mm(3,0) - mm(0,3)*mm(1,1)*mm(2,2)*mm(3,0) + mm(0,1)*mm(1,3)*mm(2,2)*mm(3,0) +
		mm(0,2)*mm(1,1)*mm(2,3)*mm(3,0) - mm(0,1)*mm(1,2)*mm(2,3)*mm(3,0) - mm(0,3)*mm(1,2)*mm(2,0)*mm(3,1) + mm(0,2)*mm(1,3)*mm(2,0)*mm(3,1) +
		mm(0,3)*mm(1,0)*mm(2,2)*mm(3,1) - mm(0,0)*mm(1,3)*mm(2,2)*mm(3,1) - mm(0,2)*mm(1,0)*mm(2,3)*mm(3,1) + mm(0,0)*mm(1,2)*mm(2,3)*mm(3,1) +
		mm(0,3)*mm(1,1)*mm(2,0)*mm(3,2) - mm(0,1)*mm(1,3)*mm(2,0)*mm(3,2) - mm(0,3)*mm(1,0)*mm(2,1)*mm(3,2) + mm(0,0)*mm(1,3)*mm(2,1)*mm(3,2) +
		mm(0,1)*mm(1,0)*mm(2,3)*mm(3,2) - mm(0,0)*mm(1,1)*mm(2,3)*mm(3,2) - mm(0,2)*mm(1,1)*mm(2,0)*mm(3,3) + mm(0,1)*mm(1,2)*mm(2,0)*mm(3,3) +
		mm(0,2)*mm(1,0)*mm(2,1)*mm(3,3) - mm(0,0)*mm(1,2)*mm(2,1)*mm(3,3) - mm(0,1)*mm(1,0)*mm(2,2)*mm(3,3) + mm(0,0)*mm(1,1)*mm(2,2)*mm(3,3);

	mat4 ret;	
	if( d == 0 ) return;
	d = 1.0f / d;

	ret(0,0) = d * (mm(1,2)*mm(2,3)*mm(3,1) - mm(1,3)*mm(2,2)*mm(3,1) + mm(1,3)*mm(2,1)*mm(3,2) - mm(1,1)*mm(2,3)*mm(3,2) - mm(1,2)*mm(2,1)*mm(3,3) + mm(1,1)*mm(2,2)*mm(3,3));
	ret(0,1) = d * (mm(0,3)*mm(2,2)*mm(3,1) - mm(0,2)*mm(2,3)*mm(3,1) - mm(0,3)*mm(2,1)*mm(3,2) + mm(0,1)*mm(2,3)*mm(3,2) + mm(0,2)*mm(2,1)*mm(3,3) - mm(0,1)*mm(2,2)*mm(3,3));
	ret(0,2) = d * (mm(0,2)*mm(1,3)*mm(3,1) - mm(0,3)*mm(1,2)*mm(3,1) + mm(0,3)*mm(1,1)*mm(3,2) - mm(0,1)*mm(1,3)*mm(3,2) - mm(0,2)*mm(1,1)*mm(3,3) + mm(0,1)*mm(1,2)*mm(3,3));
	ret(0,3) = d * (mm(0,3)*mm(1,2)*mm(2,1) - mm(0,2)*mm(1,3)*mm(2,1) - mm(0,3)*mm(1,1)*mm(2,2) + mm(0,1)*mm(1,3)*mm(2,2) + mm(0,2)*mm(1,1)*mm(2,3) - mm(0,1)*mm(1,2)*mm(2,3));
	ret(1,0) = d * (mm(1,3)*mm(2,2)*mm(3,0) - mm(1,2)*mm(2,3)*mm(3,0) - mm(1,3)*mm(2,0)*mm(3,2) + mm(1,0)*mm(2,3)*mm(3,2) + mm(1,2)*mm(2,0)*mm(3,3) - mm(1,0)*mm(2,2)*mm(3,3));
	ret(1,1) = d * (mm(0,2)*mm(2,3)*mm(3,0) - mm(0,3)*mm(2,2)*mm(3,0) + mm(0,3)*mm(2,0)*mm(3,2) - mm(0,0)*mm(2,3)*mm(3,2) - mm(0,2)*mm(2,0)*mm(3,3) + mm(0,0)*mm(2,2)*mm(3,3));
	ret(1,2) = d * (mm(0,3)*mm(1,2)*mm(3,0) - mm(0,2)*mm(1,3)*mm(3,0) - mm(0,3)*mm(1,0)*mm(3,2) + mm(0,0)*mm(1,3)*mm(3,2) + mm(0,2)*mm(1,0)*mm(3,3) - mm(0,0)*mm(1,2)*mm(3,3));
	ret(1,3) = d * (mm(0,2)*mm(1,3)*mm(2,0) - mm(0,3)*mm(1,2)*mm(2,0) + mm(0,3)*mm(1,0)*mm(2,2) - mm(0,0)*mm(1,3)*mm(2,2) - mm(0,2)*mm(1,0)*mm(2,3) + mm(0,0)*mm(1,2)*mm(2,3));
	ret(2,0) = d * (mm(1,1)*mm(2,3)*mm(3,0) - mm(1,3)*mm(2,1)*mm(3,0) + mm(1,3)*mm(2,0)*mm(3,1) - mm(1,0)*mm(2,3)*mm(3,1) - mm(1,1)*mm(2,0)*mm(3,3) + mm(1,0)*mm(2,1)*mm(3,3));
	ret(2,1) = d * (mm(0,3)*mm(2,1)*mm(3,0) - mm(0,1)*mm(2,3)*mm(3,0) - mm(0,3)*mm(2,0)*mm(3,1) + mm(0,0)*mm(2,3)*mm(3,1) + mm(0,1)*mm(2,0)*mm(3,3) - mm(0,0)*mm(2,1)*mm(3,3));
	ret(2,2) = d * (mm(0,1)*mm(1,3)*mm(3,0) - mm(0,3)*mm(1,1)*mm(3,0) + mm(0,3)*mm(1,0)*mm(3,1) - mm(0,0)*mm(1,3)*mm(3,1) - mm(0,1)*mm(1,0)*mm(3,3) + mm(0,0)*mm(1,1)*mm(3,3));
	ret(2,3) = d * (mm(0,3)*mm(1,1)*mm(2,0) - mm(0,1)*mm(1,3)*mm(2,0) - mm(0,3)*mm(1,0)*mm(2,1) + mm(0,0)*mm(1,3)*mm(2,1) + mm(0,1)*mm(1,0)*mm(2,3) - mm(0,0)*mm(1,1)*mm(2,3));
	ret(3,0) = d * (mm(1,2)*mm(2,1)*mm(3,0) - mm(1,1)*mm(2,2)*mm(3,0) - mm(1,2)*mm(2,0)*mm(3,1) + mm(1,0)*mm(2,2)*mm(3,1) + mm(1,1)*mm(2,0)*mm(3,2) - mm(1,0)*mm(2,1)*mm(3,2));
	ret(3,1) = d * (mm(0,1)*mm(2,2)*mm(3,0) - mm(0,2)*mm(2,1)*mm(3,0) + mm(0,2)*mm(2,0)*mm(3,1) - mm(0,0)*mm(2,2)*mm(3,1) - mm(0,1)*mm(2,0)*mm(3,2) + mm(0,0)*mm(2,1)*mm(3,2));
	ret(3,2) = d * (mm(0,2)*mm(1,1)*mm(3,0) - mm(0,1)*mm(1,2)*mm(3,0) - mm(0,2)*mm(1,0)*mm(3,1) + mm(0,0)*mm(1,2)*mm(3,1) + mm(0,1)*mm(1,0)*mm(3,2) - mm(0,0)*mm(1,1)*mm(3,2));
	ret(3,3) = d * (mm(0,1)*mm(1,2)*mm(2,0) - mm(0,2)*mm(1,1)*mm(2,0) + mm(0,2)*mm(1,0)*mm(2,1) - mm(0,0)*mm(1,2)*mm(2,1) - mm(0,1)*mm(1,0)*mm(2,2) + mm(0,0)*mm(1,1)*mm(2,2));

	*this=ret;
}
vec3 mat4::GetDir()
{
	vec3 dr(-m[2],-m[6],-m[10]);
	return dr;
}

vec3 mat4::GetPos()
{
	vec3 ps;
	ps.x=(m[0]*m[12]+m[1]*m[13]+m[2]*m[14]);
	ps.y=(m[4]*m[12]+m[5]*m[13]+m[6]*m[14]);
	ps.z=(m[8]*m[12]+m[9]*m[13]+m[10]*m[14]);
	return ps;
}
void mat4::GeneratePerspective(float fov,float aspect,float nearz,float farz)
{	
	float f=(float)(cos(fov/180.0f*PI/2)/sin(fov/180.0f*PI/2));
	Identity();
	m[0]=(float)(f/aspect);
	m[5]=f;
	m[10]=(float)((farz+nearz)/(nearz-farz));
	m[11]=-1;
	m[14]=(float)((2*farz*nearz)/(nearz-farz));
	m[15]=0;
}
void mat4::GenerateOrthographic(float left,float right,float bottom,float top,float zNear,float zFar)
{	
	float tx=-(right+left)/(right-left);
	float ty=-(top+bottom)/(top-bottom);
	float tz=-(zFar+zNear)/(zFar-zNear);
	Identity();
	m[0]=2/(right-left);
	m[5]=2/(top-bottom);
	m[10]=-2/(zFar-zNear);	
	m[12]=tx;
	m[13]=ty;
	m[14]=tz;

}
void mat4::LookAt(vec3 pos,vec3 at,vec3 up)
{
	mat4 trans;
	vec3 f=at-pos;

	up.Normalize();
	f.Normalize();
	vec3 s;
	vec3 u;	
	if ((up==f) || (up==f*-1))
	{
		up.Set(0,0,-1);
	}
	s.Cross(f,up);
	u.Cross(s,f);
	s.Normalize();
	u.Normalize();



	m[0]=s.x; m[4]=s.y; m[8]=s.z; m[12]=0;
	m[1]=u.x; m[5]=u.y; m[9]=u.z; m[13]=0;
	m[2]=-f.x; m[6]=-f.y; m[10]=-f.z; m[14]=0;
	m[3]=0; m[7]=0; m[11]=0; m[15]=1;

	trans.Translate(-pos.x,-pos.y,-pos.z);
	(*this)*=trans;
}
mat4::operator mat3()
{
	mat3 ret;

	ret.m[0]=m[0];
	ret.m[1]=m[1];
	ret.m[2]=m[2];
	ret.m[3]=m[4];
	ret.m[4]=m[5];
	ret.m[5]=m[6];
	ret.m[6]=m[8];
	ret.m[7]=m[9];
	ret.m[8]=m[10];

	return ret;
}
mat3 mat4::ToMatrix3()
{
	mat3 ret;

	ret.m[0]=m[0];
	ret.m[1]=m[1];
	ret.m[2]=m[2];
	ret.m[3]=m[4];
	ret.m[4]=m[5];
	ret.m[5]=m[6];
	ret.m[6]=m[8];
	ret.m[7]=m[9];
	ret.m[8]=m[10];

	return ret;
}