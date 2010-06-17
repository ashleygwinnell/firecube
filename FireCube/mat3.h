#ifndef MAT3_H
#define MAT3_H

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
	* Initializes the matrix to the identity matrix.
	*/
	mat3() {Identity();}
	inline mat3(float v11, float v21, float v31,float v12, float v22, float v32,float v13, float v23, float v33  );
	inline mat3(const vec3 & v1,const vec3 & v2, const vec3 & v3);
	/**
	* Sets all values.
	*/
	mat3(float nv) { Set(nv); }
	/**
	* Zeroes the matrix.
	*/
	void Clear() {memset(m,0,sizeof(float)*9);}
	void Set(float *src) {memcpy(m,src,sizeof(float)*9);}
	void Set(float nv) {m[0]=nv;m[1]=nv;m[2]=nv;m[3]=nv;m[4]=nv;m[5]=nv;m[6]=nv;m[7]=nv;m[8]=nv;}
	/**
	* Zeroes the matrix.
	*/
	inline void Zero();
	/**
	* Sets the matrix to the identity matrix.
	*/
	inline void Identity();	
	inline mat3 operator=(const mat3 &src);
	inline mat3 operator+(mat3 &src);
	inline mat3 operator-(mat3 &src);
	inline void operator+=(mat3 &src);
	inline void operator-=(mat3 &src);
	mat3 operator*(mat3 &src);	
	void operator*=(mat3 &src);	
	float & operator()(const unsigned int i, const unsigned int j) {return m[i + 3*j];}
	const float & operator()(const unsigned int i, const unsigned int j) const {return m[i + 3*j];}
	operator mat4();
	/**
	* Creates a scaling matrix.
	* @param x Scaling along the X axis.
	* @param y Scaling along the Y axis.
	* @param z Scaling along the Z axis.
	*/
	void Scale(float x,float y,float z);
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
	vec3 GetDir();
	/**
	* Converts to a mat4 by complementing to a 4x4 indentity matrix.
	*/
	mat4 ToMatrix4();
	/**
	* Orthonormalizes the matrix.
	*/
	inline void Orthonormalize();
	/**
	* Checks whether the matrix contain valid floating point numbers.
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
	float m[9];
};
/**
* Transposes the matrix.
*/
mat3 FIRECUBE_API Transpose(const mat3 mat);
mat3 FIRECUBE_API operator*(const mat3 &a,const float &b);
mat3 FIRECUBE_API operator*(const float &a,const mat3 &b);
vec3 FIRECUBE_API operator*(const mat3 &a,const vec3 &b);
vec3 FIRECUBE_API operator*(const vec3 &a,const mat3 &b);
}
#endif