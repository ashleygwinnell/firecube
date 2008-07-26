#ifndef VEC3_H
#define VEC3_H
/**
* A 3d vector.
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

	/**
	* Returns a pointer the the coordinates.
	*/
	inline const float * GetData() {return &x;} 
	/**
	* Returns a const pointer the the coordinates.
	*/
	inline const float * GetData() const {return &x;} 
	/**
	* Sets the vector to (0,0,0).
	*/
	void Clear() {x=y=z=0;}
	/**
	* Normalizes the vector.
	*/
	vec3 Normalize();
	/**
	* Returns the length of the vector
	*/
	float Length() const;
	/**
	* Returns the squared length of the vector
	*/
	float Length2() const;
	/**
	* Performs a dot product between two vectors.
	*/
	float Dot(const vec3 &v) const
	{return (x*v.x+y*v.y+z*v.z);}
	
	/**
	* Performs a cross product between two vectors and sets to result to this vector.
	*/
	void Cross(const vec3 &p,const vec3 &q);
	void Set(float nx,float ny,float nz)
	{x=nx;y=ny;z=nz;}
	/**
	* Sets all coordinates.
	*/
	void Set(float nv)
	{x=nv;y=nv;z=nv;}	
	/**
	* Converts to a vec2 by ignoring the z coordinate.
	*/
	vec2 ToVec2();
	
	/**
	* Sets the length of the vector.
	*/
	void SetLength(float l);
	/**
	* Rotates the vector around the X axis.
	* @param ang The amount to rotate in radians.
	*/
	void RotateX(float ang);
	/**
	* Rotates the vector around the Y axis.
	* @param ang The amount to rotate in radians.
	*/
	void RotateY(float ang);
	/**
	* Rotates the vector around the Z axis.
	* @param ang The amount to rotate in radians.
	*/
	void RotateZ(float ang);
	/**
	* Creates a vector from two angles.
	*/
	void FromAngles(float angx,float angy);
	/**
	* Transforms the vector by a matrix treating it as a vertex.<br>
	* Multiplies the 4d vector (x,y,z,1) by the matrix and then divides it by the w component.
	*/
	vec3 TransformCoordinate(mat4 m);
	/**
	* Transforms the vector by a matrix treating it as a normal.
	*/
	vec3 TransformNormal(mat4 m);
	/**
	* Checks whether the vector contain valid floating point numbers.
	*/
	inline bool Sensible() const;

	float x,y,z;	
};
vec3 FIRECUBE_API operator+(const vec3 &a,const vec3 &b);
vec3 FIRECUBE_API operator-(const vec3 &a,const vec3 &b);
vec3 FIRECUBE_API operator*(const vec3 &a,const float &b);
vec3 FIRECUBE_API operator*(const float &a,const vec3 &b);
/**
* Performs a cross product between two vectors.
*/
vec3 FIRECUBE_API Cross(const vec3 a,const vec3 b);
/**
* Performs a dot product between two vectors.
*/
float FIRECUBE_API Dot(const vec3 a,const vec3 b);
#endif