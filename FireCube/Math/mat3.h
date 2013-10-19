#pragma once

namespace FireCube
{
static inline vec3 proj(const vec3 & v1, const vec3 & v2);
/**
* A 3x3 matrix.
*/
class FIRECUBE_API mat3
{
public:
	
	/**
	* Constructs a matrix.
	*/
	mat3() 
	{

	}

	/**
	* Constructs a matrix for the specified value.
	*/
	inline mat3(float v11, float v21, float v31, float v12, float v22, float v32, float v13, float v23, float v33);

	/**
	* @brief Constructs a matrix for the specified vectors.
	*		 Each vector is assigned to a row in the matrix
	*/
	inline mat3(const vec3 & v1, const vec3 & v2, const vec3 & v3);

	/**
	* Sets all values.
	*/
	mat3(float nv) {
		Set(nv);
	}

	/**
	* Zeroes the matrix.
	*/
	void Clear() {
		memset(m, 0, sizeof(float) * 9);
	}

	/**
	* Copies an array into the matrix.
	* @param src A pointer to the array to copy.
	*/
	void Set(float *src) {
		memcpy(m, src, sizeof(float) * 9);
	}

	/**
	* Sets all elements of the matrix to a given value.
	* @param nv The value to assign to the elements of the matrix.
	*/
	void Set(float nv) {
		m[0] = nv;
		m[1] = nv;
		m[2] = nv;
		m[3] = nv;
		m[4] = nv;
		m[5] = nv;
		m[6] = nv;
		m[7] = nv;
		m[8] = nv;
	}

	/**
	* Zeroes the matrix.
	*/
	inline void Zero();
	/**
	* Sets the matrix to the identity matrix.
	*/
	inline void Identity();

	/**
	* Assigns a matrix to another.
	* @param src The matrix to copy.
	*/
	inline mat3 operator=(const mat3 &src);

	/**
	* Adds two matrices.
	*/
	inline mat3 operator+(const mat3 &src) const;

	/**
	* Subtracts two matrices.
	*/
	inline mat3 operator-(const mat3 &src) const;

	/**
	* Adds two matrices and assign to this one.
	*/
	inline void operator+=(const mat3 &src);

	/**
	* Subtracts two matrices and assign to this one.
	*/
	inline void operator-=(const mat3 &src);

	/**
	* Multiplies two matrices.
	*/
	mat3 operator*(const mat3 &src) const;

	/**
	* Multiplies two matrices and assign to this one.
	*/
	void operator*=(const mat3 &src);

	/**
	* Gets an element in the matrix.
	* @return The specified element.
	*/
	float & operator()(const unsigned int i, const unsigned int j) {
		return m[i + 3 * j];
	}

	/**
	* Gets an element in the matrix.
	* @return The specified element.
	*/
	const float & operator()(const unsigned int i, const unsigned int j) const {
		return m[i + 3 * j];
	}

	/**
	* Creates a scaling matrix.
	* @param x Scaling along the X axis.
	* @param y Scaling along the Y axis.
	* @param z Scaling along the Z axis.
	*/
	void Scale(float x, float y, float z);

	/**
	* Creates a rotation matrix along the X axis.
	* @param ang The amount to rotate in radians.
	*/
	void RotateX(float ang);

	/**
	* Creates a rotation matrix along the Y axis.
	* @param ang The amount to rotate in radians.
	*/
	void RotateY(float ang);

	/**
	* Creates a rotation matrix along the Z axis.
	* @param ang The amount to rotate in radians.
	*/
	void RotateZ(float ang);

	/**
	* Transposes the matrix.
	*/
	void Transpose();

	/**
	* Inverts the matrix.
	*/
	void Inverse();

	/**
	* Returns the direction of the look at vector.
	*/
	vec3 GetDirection() const;

	/**
	* Converts to a mat4 by complementing to a 4x4 identity matrix.
	*/
	mat4 ToMat4() const;

	/**
	* Orthonormalizes the matrix.
	*/
	inline void Orthonormalize();

	/**
	* Checks whether the matrix contains valid floating point numbers.
	*/
	inline bool Sensible() const;

	/**
	* Returns a column.
	* @param i The column number.
	*/
	inline vec3 GetCol(unsigned int i) const;

	/** Sets a column.
	* @param i The column number.
	* @param col The vec3 representing the column.
	*/
	inline void SetCol(unsigned int i, const vec3 & col);

	/**
	* Extract the Euler angles for this matrix.
	* @return A vector containing the three angles of rotation.
	*/
	vec3 ExtractEulerAngles() const;

	/**
	* The elements of the matrix;
	*/
	float m[9];

	/**
	* A 3x3 identity matrix.
	*/
	static const mat3 IDENTITY;
};

/**
* Multiplies a matrix with a scalar.
*/
mat3 FIRECUBE_API operator*(const mat3 &a, float b);

/**
* Multiplies a matrix with a scalar.
*/
mat3 FIRECUBE_API operator*(float a, const mat3 &b);

/**
* Multiplies a matrix with a vector.
*/
vec3 FIRECUBE_API operator*(const mat3 &a, const vec3 &b);

/**
* Multiplies a matrix with a vector.
*/
vec3 FIRECUBE_API operator*(const vec3 &a, const mat3 &b);
}
