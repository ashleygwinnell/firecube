#ifndef VEC2_H
#define VEC2_H
class FIRECUBE_API vec2
{
public:
	vec2() {}
	vec2(float nx,float ny) {x=nx;y=ny;}
	vec2(const vec2 &v) {x=v.x;y=v.y;}

	vec2 operator=(const vec2 &src)
	{return vec2(x=src.x,y=src.y);}

	vec2 operator=(vec2 *src)
	{return vec2(x=src->x,y=src->y);}

	int operator==(vec2 &src)
	{return (x==src.x && y==src.y);}

	int operator==(vec2 *src)
	{return (x==src->x && y==src->y);}

	int operator!=(vec2 &src)
	{return !(src==(*this));}

	int operator!=(vec2 *src)
	{return !(src==this);}

	void operator+=(const vec2 &src);
	void operator-=(vec2 &src);
	void operator*=(vec2 &src);
	void operator*=(float val);
	void operator/=(vec2 &src);
	void operator/=(float val);

	vec2 operator+(const vec2 &src)
	{return vec2(x+src.x,y+src.y);}

	vec2 operator-(vec2 &src)
	{return vec2(x-src.x,y-src.y);}	

	vec2 operator*(vec2 &src)
	{return vec2(x*src.x,y*src.y);}

	vec2 operator*(float val)
	{return vec2(x*val,y*val);}

	vec2 operator*(mat4 &src);


	vec2 operator/(vec2 &src)
	{return vec2(x/src.x,y/src.y);}

	vec2 operator/(float val)
	{return vec2(x/val,y/val);}

	vec2 operator-(void) {return vec2(-x, -y);}
	float & operator[](unsigned int i);
	const float & operator[](unsigned int i) const;
	float & operator()(unsigned int i);
	const float & operator()(unsigned int i) const;

	inline vec2 operator-() const {return vec2(-x, -y);}
	operator vec3();

	inline const float * GetData() {return &x;} 
	inline const float * GetData() const {return &x;} 
	void Clear() {x=y=0;}
	vec2 Normalize();
	float Length() const;
	float Length2() const;
	float Dot(const vec2 &v) const
	{return (x*v.x+y*v.y);}

	float Cross(const vec2 &p,const vec2 &q);
	void Set(float nx,float ny)
	{x=nx;y=ny;}
	void Set(float nv)
	{x=nv;y=nv;}	
	vec3 ToVec3();

	void SetLength(float l);
	void Rotate(float ang);
	void FromAngle(float ang);
	vec2 TransformCoordinate(mat4 m);
	vec2 TransformNormal(mat4 m);
	inline bool Sensible() const;
	
	float x,y;	
};
vec2 FIRECUBE_API operator+(const vec2 &a,const vec2 &b);
vec2 FIRECUBE_API operator-(const vec2 &a,const vec2 &b);
vec2 FIRECUBE_API operator*(const vec2 &a,const float &b);
vec2 FIRECUBE_API operator*(const float &a,const vec2 &b);
float FIRECUBE_API Cross(const vec2 a,const vec2 b);
float FIRECUBE_API Dot(const vec2 a,const vec2 b);
#endif