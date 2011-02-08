#ifndef VEC4_H
#define VEC4_H

namespace FireCube
{
/**
* A 4d vector.
*/
class FIRECUBE_API vec4
{
public:
	vec4() {}

	/**
	* Constructs a vector for the specified value.
	*/
	vec4(float nx,float ny,float nz,float nw) {x=nx;y=ny;z=nz;w=nw;}

	/**
	* Constructs a vector from another.
	* @param v The vector to copy.
	*/
	vec4(const vec4 &v) {x=v.x;y=v.y;z=v.z;w=v.w;}

	/**
	* Assigns a vector to another.
	* @param src The vector to copy.
	*/
	vec4 operator=(const vec4 &src)
	{return vec4(x=src.x,y=src.y,z=src.z,w=src.w);}

	/**
	* Checks whether two vectors are equal.
	* @return True if the two vectors are equal, false otherwise.
	*/
	int operator==(vec4 &src)
	{return (x==src.x && y==src.y && z==src.z && w==src.w);}

	/**
	* Checks whether two vectors are not equal.
	* @return True if the two vectors are not equal, false otherwise.
	*/
	int operator!=(vec4 &src)
	{return !(src==(*this));}

	/**
	* Adds two vectors and assign to this one.	
	*/
	void operator+=(const vec4 &src);

	/**
	* Subtracts two vectors and assign to this one.	
	*/
	void operator-=(vec4 &src);

	/**
	* Multiplies two vectors(component-wise) and assign to this one.	
	*/
	void operator*=(vec4 &src);

	/**
	* Multiplies the vector with a scalar and assign to this one.	
	*/
	void operator*=(float val);

	/**
	* Divides two vectors(component-wise) and assign to this one.	
	*/
	void operator/=(vec4 &src);

	/**
	* Divides the vector with a scalar and assign to this one.	
	*/
	void operator/=(float val);

	/**
	* Adds two vectors.	
	*/
	vec4 operator+(const vec4 &src)
	{return vec4(x+src.x,y+src.y,z+src.z,w+src.w);}

	/**
	* Subtracts two vectors.	
	*/
	vec4 operator-(vec4 &src)
	{return vec4(x-src.x,y-src.y,z-src.z,w-src.w);}	

	/**
	* Multiplies two vectors(component-wise).
	*/
	vec4 operator*(vec4 &src)
	{return vec4(x*src.x,y*src.y,z*src.z,w*src.w);}

	/**
	* Multiplies a vector with a scalar.
	*/
	vec4 operator*(float val)
	{return vec4(x*val,y*val,z*val,w*val);}

	/**
	* Multiplies a vector with a 4d matrix.
	*/
	vec4 operator*(mat4 &src);

	/**
	* Divides two vectors(component-wise).
	*/
	vec4 operator/(vec4 &src)
	{return vec4(x/src.x,y/src.y,z/src.z,w/src.w);}

	/**
	* Divides a vector with a scalar.
	*/
	vec4 operator/(float val)
	{return vec4(x/val,y/val,z/val,w/val);}

	/**
	* Negates a vector.
	*/
	vec4 operator-(void) {return vec4(-x, -y, -z,-w);}

	/**
	* Indexes the vector.
	*/
	float & operator[](unsigned int i);

	/**
	* Indexes the vector.
	*/
	const float & operator[](unsigned int i) const;

	/**
	* Indexes the vector.
	*/
	float & operator()(unsigned int i);

	/**
	* Indexes the vector.
	*/
	const float & operator()(unsigned int i) const;

	/**
	* Negates a vector.
	*/
	inline vec4 operator-() const {return vec4(-x, -y, -z,-w);}

	/**
	* Returns a pointer to the coordinates.
	*/
	inline const float * GetData() {return &x;} 
	
	/**
	* Returns a const pointer to the coordinates.
	*/
	inline const float * GetData() const {return &x;} 
	
	/**
	* Sets the vector to (0,0,0).
	*/
	void Clear() {x=y=z=w=0;}
	
	/**
	* Normalizes the vector.
	*/
	vec4 Normalize();
	
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
	float Dot(const vec4 &v) const
	{return (x*v.x+y*v.y+z*v.z+w*v.w);}

	/**
	* Performs a cross product between two vectors and sets to result to this vector.
	*/
	void Cross(const vec4 &p,const vec4 &q);

	/**
	* Sets the coordinates of the vector.
	*/
	void Set(float nx,float ny,float nz,float nw)
	{x=nx;y=ny;z=nz;w=nw;}
	
	/**
	* Sets all coordinates.
	*/
	void Set(float nv)
	{x=nv;y=nv;z=nv;w=nv;}	
	
	/**
	* Converts to a vec2 by ignoring the z,w coordinates.
	*/
	vec2 ToVec2();
	
	/**
	* Converts to a vec3 by ignoring the w coordinate.
	*/
	vec3 ToVec3();
	
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
	* Checks whether the vector contain valid floating point numbers.
	*/
	inline bool Sensible() const;

	/**
	* X coordinate.
	*/
	float x;

	/**
	* Y coordinate.
	*/
	float y;

	/**
	* Z coordinate.
	*/
	float z;

	/**
	* W coordinate.
	*/
	float w;
};
vec4 FIRECUBE_API operator+(const vec4 &a,const vec4 &b);
vec4 FIRECUBE_API operator-(const vec4 &a,const vec4 &b);
vec4 FIRECUBE_API operator*(const vec4 &a,const float &b);
vec4 FIRECUBE_API operator*(const float &a,const vec4 &b);
/**
* Performs a cross product between two vectors.
*/
vec4 FIRECUBE_API Cross(const vec4 a,const vec4 b);
/**
* Performs a dot product between two vectors.
*/
float FIRECUBE_API Dot(const vec4 a,const vec4 b);
}
#endif