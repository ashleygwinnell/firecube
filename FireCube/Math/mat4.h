#ifndef MAT4_H
#define MAT4_H

namespace FireCube
{
/**
* A 4x4 matrix.
*/
class FIRECUBE_API mat4
{
public:
	
	/**
	* Constructs a matrix.
	*/
	mat4()
	{        

	}

	/**
	* Constructs a matrix for the specified value.
	*/
	inline mat4(float v11, float v21, float v31, float v41, float v12, float v22, float v32, float v42, float v13, float v23, float v33, float v43, float v14, float v24, float v34, float v44);

	/**
	* Constructs a matrix from a value.
	* @param nv The value to assign to each element of the matrix.
	*/
	mat4(float nv) {
		Set(nv);
	}

	/**
	* Zeroes the matrix.
	*/
	void Clear() {
		memset(m, 0, sizeof(float) * 16);
	}

	/**
	* Copies an array into the matrix.
	* @param src A pointer to the array to copy.
	*/
	void Set(float *src) {
		memcpy(m, src, sizeof(float) * 16);
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
		m[9] = nv;
		m[10] = nv;
		m[11] = nv;
		m[12] = nv;
		m[13] = nv;
		m[14] = nv;
		m[15] = nv;
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
	inline mat4 operator=(const mat4 &src);

	/**
	* Adds two matrices.
	*/
	inline mat4 operator+(const mat4 &src) const;

	/**
	* Subtracts two matrices.
	*/
	inline mat4 operator-(const mat4 &src) const;

	/**
	* Adds two matrices and assign to this one.
	*/
	inline void operator+=(const mat4 &src);

	/**
	* Subtracts two matrices and assign to this one.
	*/
	inline void operator-=(const mat4 &src);

	/**
	* Multiplies two matrices.
	*/
	mat4 operator*(const mat4 &src) const;

	/**
	* Multiplies a matrix with a scalar.
	*/
	mat4 operator*(float src) const;

	/**
	* Multiplies two matrices and assign to this one.
	*/
	void operator*=(const mat4 &src);

	/**
	* Multiplies a matrix with a 3d vector.
	*/
	vec3 operator*(const vec3 &src) const;

	/**
	* Multiplies a matrix with a 4d vector.
	*/
	vec4 operator*(const vec4 &src) const;

	/**
	* Gets an element in the matrix.
	* @return The specified element.
	*/
	float & operator()(const unsigned int i, const unsigned int j) {
		return m[i + 4 * j];
	}

	/**
	* Gets an element in the matrix.
	* @return The specified element.
	*/
	const float & operator()(const unsigned int i, const unsigned int j) const {
		return m[i + 4 * j];
	}

	/**
	* Creates a translation matrix.
	* @param x Translation along the X axis.
	* @param y Translation along the Y axis.
	* @param z Translation along the Z axis.
	*/
	void Translate(float x, float y, float z);

	/**
	* Creates a translation matrix.
	* @param t A vec3 representing the translation.
	*/
	void Translate(const vec3 &t);

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
	* Creates a rotation matrix from an axis angle.
	* @param axis The axis of rotation.
	* @param angle Then angle (in radians).
	*/
	void Rotate(const vec3 &axis, float angle);

	/**
	* Transposes the matrix.
	*/
	void Transpose();

	/**
	* Inverts the matrix.
	*/
	void Inverse();

	/**
	* Returns the translation in the matrix.
	*/
	vec3 GetPosition() const;

	/**
	* Returns the direction of the look at vector.
	*/
	vec3 GetDirection() const;

	/**
	* Generates a perspective projection matrix.
	* @param fov The field of view in degrees.
	* @param aspect The aspect ratio.
	* @param nearz The near clipping plane.
	* @param farz The far clipping plane.
	*/
	void GeneratePerspective(float fov, float aspect, float nearz, float farz);

	/**
	* Generates an orthographic projection matrix.
	* @param left The left side of the view.
	* @param right The right side of the view.
	* @param bottom The bottom side of the view.
	* @param top The top side of the view.
	* @param zNear The near clipping plane.
	* @param zFar The far clipping plane.
	*/
	void GenerateOrthographic(float left, float right, float bottom, float top, float zNear, float zFar);

	/**
	* Generates a look-at matrix.
	* @param pos Position of the camera.
	* @param at The look-at location of the camera.
	* @param up The up axis for the camera.
	*/
	void LookAt(vec3 pos, vec3 at, vec3 up);

	/**
	* Extract the Euler angles for this matrix.
	* @return A vector containing the three angles of rotation.
	*/
	vec3 ExtractEulerAngles() const;

	/**
	* Converts to a mat3 by taking the upper 3x3 part.
	*/
	mat3 ToMat3() const;

	/**
	* Returns the determinant of the matrix
	*/
	float Determinant() const;

	/**
	* Decomposes the matrix into a scaling vector, translation vector and rotation matrix
	*/
	void Decompose(vec3 &scaling, vec3 &translation, mat3 &rotation) const;

	/**
	* The elements of the matrix;
	*/
	float m[16];

	/**
	* A 4x4 identity matrix.
	*/
	static const mat4 identity;
};
}
#endif