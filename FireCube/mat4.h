#ifndef MAT4_H
#define MAT4_H
/**
* A 4x4 matrix.
*/
class FIRECUBE_API mat4
{
public:
	/**
	* Initializes the matrix to the identity matrix.
	*/
	mat4() {Identity();}
	mat4(float nv) { Set(nv); }
	/**
	* Zeroes the matrix.
	*/
	void Clear() {memset(m,0,sizeof(float)*16);}
	void Set(float *src) {memcpy(m,src,sizeof(float)*16);}
	/**
	* Sets all values.
	*/
	void Set(float nv) {m[0]=nv;m[1]=nv;m[2]=nv;m[3]=nv;m[4]=nv;m[5]=nv;m[6]=nv;m[7]=nv;m[8]=nv;m[9]=nv;m[10]=nv;m[11]=nv;m[12]=nv;m[13]=nv;m[14]=nv;m[15]=nv;}
	/**
	* Zeroes the matrix.
	*/
	inline void Zero();
	/**
	* Sets the matrix to the identity matrix.
	*/
	inline void Identity();	
	inline mat4 operator=(const mat4 &src);
	inline mat4 operator+(mat4 &src);
	inline mat4 operator-(mat4 &src);
	inline void operator+=(mat4 &src);
	inline void operator-=(mat4 &src);
	mat4 operator*(mat4 &src);
	mat4 operator*(float src);
	void operator*=(mat4 &src);
	vec3 operator*(vec3 &src);
	float & operator()(const unsigned int i, const unsigned int j) {return m[i + 4*j];}
	const float & operator()(const unsigned int i, const unsigned int j) const {return m[i + 4*j];}
	operator mat3();
	/**
	* Creates a translation matrix.
	* @param x Translation along the X axis.
	* @param y Translation along the Y axis.
	* @param z Translation along the Z axis.
	*/
	void Translate(float x,float y,float z);
	/**
	* Creates a translation matrix.
	* @param t A vec3 representing the translation.
	*/
	void Translate(vec3 t);
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
	* Creates a rotation matrix from an axis angle.
	* @param rot The axis angle(x,y,z,a).
	*/
	void Rotate(vec4 rot);
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
	vec3 GetPos();
	/**
	* Returns the direction of the look at vector.
	*/
	vec3 GetDir();
	/**
	* Generates a perspective projection matrix.
	* @param fov The field of view in degrees.
	* @param aspect The aspect ratio.
	* @param nearz The near clipping plane.
	* @param farz The far clipping plane.
	*/
	void GeneratePerspective(float fov,float aspect,float nearz,float farz);
	/**
	* Generates a orthographic projection matrix.
	* @param left The left side of the view.
	* @param right The right side of the view.
	* @param bottom The bottom side of the view.
	* @param top The top side of the view.	
	* @param zNear The near clipping plane.
	* @param zFar The far clipping plane.
	*/
	void GenerateOrthographic(float left,float right,float bottom,float top,float zNear,float zFar);
	/**
	* Generates a look-at matrix.
	* @param pos Position of the camera.
	* @param at The look-at location of the camera.
	* @param up The up axis for the camera.
	*/
	void LookAt(vec3 pos,vec3 at,vec3 up);
	/*
	* Converts to a mat3 by stripping the translation part.
	*/
	mat3 ToMatrix3();
	float m[16];
};
#endif