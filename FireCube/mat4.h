#ifndef MAT4_H
#define MAT4_H
/**
A 4x4 matrix.
*/
class FIRECUBE_API mat4
{
public:
	mat4() {Identity();}
	mat4(float nv) { Set(nv); }
	void Clear() {memset(m,0,sizeof(float)*16);}
	void Set(float *src) {memcpy(m,src,sizeof(float)*16);}
	void Set(float nv) {m[0]=nv;m[1]=nv;m[2]=nv;m[3]=nv;m[4]=nv;m[5]=nv;m[6]=nv;m[7]=nv;m[8]=nv;m[9]=nv;m[10]=nv;m[11]=nv;m[12]=nv;m[13]=nv;m[14]=nv;m[15]=nv;}
	inline void Zero();
	inline void Identity();
	inline mat4 operator=(mat4 &src);
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
	void Translate(float x,float y,float z);
	void Translate(vec3 t);
	void Scale(float x,float y,float z);
	void RotateX(float ang);
	void RotateY(float ang);
	void RotateZ(float ang);
	void Transpose();
	void Inverse();	
	vec3 GetPos();
	vec3 GetDir();
	void GeneratePerspective(float fov,float aspect,float nearz,float farz);
	void GenerateOrthographic(float left,float right,float bottom,float top,float zNear,float zFar);
	void LookAt(vec3 pos,vec3 at,vec3 up);
	mat3 ToMatrix3();
	float m[16];
};
#endif