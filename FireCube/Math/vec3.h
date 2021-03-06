#pragma once

namespace FireCube
{
/**
* A 3d vector.
*/
class FIRECUBE_API vec3
{
public:
	vec3() {}

	/**
	* Constructs a vector for the specified value.
	*/
	vec3(float nx, float ny, float nz) 
	{
		x = nx;
		y = ny;
		z = nz;
	}

	/**
	* Constructs a vector for the specified value.
	*/
	vec3(float v) 
	{
		x = v;
		y = v;
		z = v;
	}

	/**
	* Constructs a vector from another.
	* @param v The vector to copy.
	*/
	vec3(const vec3 &v) 
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	vec3(const vec2 &v, float nz)
	{
		x = v.x;
		y = v.y;
		z = nz;
	}

	/**
	* Assigns a vector to another.
	* @param src The vector to copy.
	*/
	vec3 operator=(const vec3 &src)
	{
		return vec3(x = src.x, y = src.y, z = src.z);
	}

	/**
	* Checks whether two vectors are equal.
	* @return True if the two vectors are equal, false otherwise.
	*/
	bool operator==(const vec3 &src) const
	{
		return (x == src.x && y == src.y && z == src.z);
	}

	/**
	* Checks whether two vectors are not equal.
	* @return True if the two vectors are not equal, false otherwise.
	*/
	int operator!=(const vec3 &src) const
	{
		return !(src == (*this));
	}

	/**
	* Adds two vectors and assign to this one.
	*/
	void operator+=(const vec3 &src);

	/**
	* Subtracts two vectors and assign to this one.
	*/
	void operator-=(const vec3 &src);

	/**
	* Multiplies two vectors(component-wise) and assign to this one.
	*/
	void operator*=(const vec3 &src);

	/**
	* Multiplies the vector with a scalar and assign to this one.
	*/
	void operator*=(float val);

	/**
	* Divides two vectors(component-wise) and assign to this one.
	*/
	void operator/=(const vec3 &src);

	/**
	* Divides the vector with a scalar and assign to this one.
	*/
	void operator/=(float val);

	/**
	* Multiplies two vectors(component-wise).
	*/
	vec3 operator*(const vec3 &src) const
	{
		return vec3(x * src.x, y * src.y, z * src.z);
	}

	/**
	* Multiplies a vector with a 4d matrix.
	*/
	vec3 operator*(const mat4 &src) const;

	/**
	* Divides two vectors(component-wise).
	*/
	vec3 operator/(const vec3 &src) const
	{
		return vec3(x / src.x, y / src.y, z / src.z);
	}

	/**
	* Negates a vector.
	*/
	vec3 operator-(void) {
		return vec3(-x, -y, -z);
	}

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
	inline vec3 operator-() const {
		return vec3(-x, -y, -z);
	}

	/**
	* Returns a pointer to the coordinates.
	*/
	inline const float * GetData() {
		return &x;
	}

	/**
	* Returns a const pointer to the coordinates.
	*/
	inline const float * GetData() const {
		return &x;
	}

	/**
	* Sets the vector to (0,0,0).
	*/
	void Clear() {
		x = y = z = 0;
	}

	/**
	* Normalizes the vector.
	*/
	void Normalize();

	/**
	* Returns the vector normalized.
	*/
	vec3 Normalized() const;

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
	{
		return (x * v.x + y * v.y + z * v.z);
	}

	/**
	* Performs a cross product between two vectors and sets to result to this vector.
	*/
	void Cross(const vec3 &p, const vec3 &q);

	/**
	* Sets the coordinates of the vector.
	*/
	void Set(float nx, float ny, float nz)
	{
		x = nx;
		y = ny;
		z = nz;
	}

	/**
	* Sets all coordinates.
	*/
	void Set(float nv)
	{
		x = nv;
		y = nv;
		z = nv;
	}

	/**
	* Converts to a vec2 by ignoring the z coordinate.
	*/
	vec2 ToVec2() const;

	/**
	* Converts to a vec4 by setting the w component to zero.
	*/
	vec4 ToVec4() const;

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
	void FromAngles(float angx, float angy);

	/**
	* Transforms the vector by a matrix treating it as a vertex.<br>
	* Multiplies the 4d vector (x,y,z,1) by the matrix and then divides it by the w component.
	*/
	vec3 TransformCoordinate(const mat4 &m) const;

	/**
	* Transforms the vector by a matrix treating it as a normal.
	*/
	vec3 TransformNormal(const mat4 &m) const;

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

	static const vec3 ZERO;
};

/**
* Adds two vectors.
*/
vec3 FIRECUBE_API operator+(const vec3 &a, const vec3 &b);

/**
* Subtracts two vectors.
*/
vec3 FIRECUBE_API operator-(const vec3 &a, const vec3 &b);

/**
* Multiplies a vector with a scalar.
*/
vec3 FIRECUBE_API operator*(const vec3 &a, float b);

/**
* Multiplies a vector with a scalar.
*/
vec3 FIRECUBE_API operator*(float a, const vec3 &b);

/**
* Divides a vector with a scalar.
*/
vec3 FIRECUBE_API operator/(const vec3 &a, float b);

/**
* Divides a vector with a scalar.
*/
vec3 FIRECUBE_API operator/(float a, const vec3 &b);

/**
* Performs a cross product between two vectors.
*/
vec3 FIRECUBE_API Cross(const vec3 &a, const vec3 &b);

/**
* Performs a dot product between two vectors.
*/
float FIRECUBE_API Dot(const vec3 &a, const vec3 &b);
}
