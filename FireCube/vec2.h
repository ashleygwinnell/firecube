#ifndef VEC2_H
#define VEC2_H
namespace FireCube
{
/**
* A 2d vector.
*/
class FIRECUBE_API vec2
{
public:
	vec2() {}

	/**
	* Constructs a vector for the specified value.
	*/
	vec2(float nx,float ny) {x=nx;y=ny;}

	/**
	* Constructs a vector from another.
	* @param v The vector to copy.
	*/
	vec2(const vec2 &v) {x=v.x;y=v.y;}

	/**
	* Assigns a vector to another.
	* @param src The vector to copy.
	*/
	vec2 operator=(const vec2 &src)
	{return vec2(x=src.x,y=src.y);}

	/**
	* Checks whether two vectors are equal.
	* @return True if the two vectors are equal, false otherwise.
	*/
	int operator==(vec2 &src)
	{return (x==src.x && y==src.y);}

	/**
	* Checks whether two vectors are not equal.
	* @return True if the two vectors are not equal, false otherwise.
	*/
	int operator!=(vec2 &src)
	{return !(src==(*this));}

	/**
	* Adds two vectors and assign to this one.	
	*/
	void operator+=(const vec2 &src);

	/**
	* Subtracts two vectors and assign to this one.	
	*/
	void operator-=(vec2 &src);

	/**
	* Multiplies two vectors(component-wise) and assign to this one.	
	*/
	void operator*=(vec2 &src);

	/**
	* Multiplies the vector with a scalar and assign to this one.	
	*/
	void operator*=(float val);

	/**
	* Divides two vectors(component-wise) and assign to this one.	
	*/
	void operator/=(vec2 &src);

	/**
	* Divides the vector with a scalar and assign to this one.	
	*/
	void operator/=(float val);

	/**
	* Adds two vectors.	
	*/
	vec2 operator+(const vec2 &src)
	{return vec2(x+src.x,y+src.y);}

	/**
	* Subtracts two vectors.	
	*/
	vec2 operator-(vec2 &src)
	{return vec2(x-src.x,y-src.y);}	

	/**
	* Multiplies two vectors(component-wise).
	*/
	vec2 operator*(vec2 &src)
	{return vec2(x*src.x,y*src.y);}

	/**
	* Multiplies a vector with a scalar.
	*/
	vec2 operator*(float val)
	{return vec2(x*val,y*val);}

	/**
	* Multiplies a vector with a 4d matrix.
	*/
	vec2 operator*(mat4 &src);

	/**
	* Divides two vectors(component-wise).
	*/
	vec2 operator/(vec2 &src)
	{return vec2(x/src.x,y/src.y);}

	/**
	* Divides a vector with a scalar.
	*/
	vec2 operator/(float val)
	{return vec2(x/val,y/val);}

	/**
	* Negates a vector.
	*/
	vec2 operator-(void) {return vec2(-x, -y);}

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
	inline vec2 operator-() const {return vec2(-x, -y);}
	
	/**
	* Returns a pointer to the coordinates.
	*/
	inline const float * GetData() {return &x;} 
	
	/**
	* Returns a const pointer to the coordinates.
	*/
	inline const float * GetData() const {return &x;} 
	
	/**
	* Sets the vector to (0,0).
	*/
	void Clear() {x=y=0;}
	
	/**
	* Normalizes the vector.
	*/
	vec2 Normalize();
	
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
	float Dot(const vec2 &v) const
	{return (x*v.x+y*v.y);}

	/**
	* Performs a cross product between two vectors.
	*/
	float Cross(const vec2 &p,const vec2 &q);
	
	/**
	* Sets the coordinates of the vector.
	*/
	void Set(float nx,float ny)
	{x=nx;y=ny;}
	
	/**
	* Sets all coordinates.
	*/
	void Set(float nv)
	{x=nv;y=nv;}
	
	/**
	* Converts to a vec3 by setting the z component to zero.
	*/
	vec3 ToVec3();
	
	/**
	* Converts to a vec4 by setting the z,w components to zero.
	*/
	vec4 ToVec4();
	
	/**
	* Sets the length of the vector.
	*/
	void SetLength(float l);
	
	/**
	* Rotates the vector.
	* @param ang The amount to rotate in radians.
	*/
	void Rotate(float ang);
	
	/**
	* Creates a vector from an angle.
	*/
	void FromAngle(float ang);
	
	/**
	* Transforms the vector by a matrix treating it as a vertex.<br>
	* Multiplies the 3d vector (x,y,1) by the matrix and then divides it by the w component.
	*/
	vec2 TransformCoordinate(mat4 m);
	
	/**
	* Transforms the vector by a matrix treating it as a normal.
	*/
	vec2 TransformNormal(mat4 m);
	
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
};
vec2 FIRECUBE_API operator+(const vec2 &a,const vec2 &b);
vec2 FIRECUBE_API operator-(const vec2 &a,const vec2 &b);
vec2 FIRECUBE_API operator*(const vec2 &a,const float &b);
vec2 FIRECUBE_API operator*(const float &a,const vec2 &b);
/**
* Performs a cross product between two vectors.
*/
float FIRECUBE_API Cross(const vec2 a,const vec2 b);
/**
* Performs a dot product between two vectors.
*/
float FIRECUBE_API Dot(const vec2 a,const vec2 b);
}
#endif