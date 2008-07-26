#ifndef VEC3_H
#define VEC3_H
/**
A 3d vector.
*/
class FIRECUBE_API vec3
{
public:
	vec3() {}
	vec3(float nx,float ny,float nz) {x=nx;y=ny;z=nz;}
	vec3(const vec3 &v) {x=v.x;y=v.y;z=v.z;}

	vec3 operator=(const vec3 &src)
	{return vec3(x=src.x,y=src.y,z=src.z);}

	vec3 operator=(vec3 *src)
	{return vec3(x=src->x,y=src->y,z=src->z);}

	int operator==(vec3 &src)
	{return (x==src.x && y==src.y && z==src.z);}

	int operator==(vec3 *src)
	{return (x==src->x && y==src->y && z==src->z);}

	int operator!=(vec3 &src)
	{return !(src==(*this));}

	int operator!=(vec3 *src)
	{return !(src==this);}

	void operator+=(const vec3 &src);
	void operator-=(vec3 &src);
	void operator*=(vec3 &src);
	void operator*=(float val);
	void operator/=(vec3 &src);
	void operator/=(float val);

	vec3 operator+(const vec3 &src)
	{return vec3(x+src.x,y+src.y,z+src.z);}

	vec3 operator-(vec3 &src)
	{return vec3(x-src.x,y-src.y,z-src.z);}	

	vec3 operator*(vec3 &src)
	{return vec3(x*src.x,y*src.y,z*src.z);}

	vec3 operator*(float val)
	{return vec3(x*val,y*val,z*val);}

	vec3 operator*(mat4 &src);


	vec3 operator/(vec3 &src)
	{return vec3(x/src.x,y/src.y,z/src.z);}

	vec3 operator/(float val)
	{return vec3(x/val,y/val,z/val);}

	vec3 operator-(void) {return vec3(-x, -y, -z);}
	float & operator[](unsigned int i);
	const float & operator[](unsigned int i) const;
	float & operator()(unsigned int i);
	const float & operator()(unsigned int i) const;

	inline vec3 operator-() const {return vec3(-x, -y, -z);}
	operator vec2();

	inline const float * GetData() {return &x;} 
	inline const float * GetData() const {return &x;} 
	void Clear() {x=y=z=0;}
	vec3 Normalize();
	float Length() const;
	float Length2() const;
	float Dot(const vec3 &v) const
	{return (x*v.x+y*v.y+z*v.z);}

	void Cross(const vec3 &p,const vec3 &q);
	void Set(float nx,float ny,float nz)
	{x=nx;y=ny;z=nz;}
	void Set(float nv)
	{x=nv;y=nv;z=nv;}	
	vec2 ToVec2();

	void SetLength(float l);
	void RotateX(float ang);
	void RotateY(float ang);
	void RotateZ(float ang);
	void FromAngles(float angx,float angy);
	vec3 TransformCoordinate(mat4 m);
	vec3 TransformNormal(mat4 m);
	inline bool Sensible() const;

	float x,y,z;	
};
vec3 FIRECUBE_API operator+(const vec3 &a,const vec3 &b);
vec3 FIRECUBE_API operator-(const vec3 &a,const vec3 &b);
vec3 FIRECUBE_API operator*(const vec3 &a,const float &b);
vec3 FIRECUBE_API operator*(const float &a,const vec3 &b);
vec3 FIRECUBE_API Cross(const vec3 a,const vec3 b);
float FIRECUBE_API Dot(const vec3 a,const vec3 b);
#endif