#ifndef MAT3_H
#define MAT3_H
static inline vec3 proj(const vec3 & v1, const vec3 & v2);
/**
A 3x3 matrix.
*/
class FIRECUBE_API mat3
{
public:
	mat3() {Identity();}
	inline mat3(float v11, float v21, float v31,float v12, float v22, float v32,float v13, float v23, float v33  );
	inline mat3(const vec3 & v1,const vec3 & v2, const vec3 & v3);
	mat3(float nv) { Set(nv); }	
	void Clear() {memset(m,0,sizeof(float)*9);}
	void Set(float *src) {memcpy(m,src,sizeof(float)*9);}
	void Set(float nv) {m[0]=nv;m[1]=nv;m[2]=nv;m[3]=nv;m[4]=nv;m[5]=nv;m[6]=nv;m[7]=nv;m[8]=nv;}
	inline void Zero();
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
	void Scale(float x,float y,float z);
	void RotateX(float ang);
	void RotateY(float ang);
	void RotateZ(float ang);
	void Transpose();
	void Inverse();	
	vec3 GetDir();
	mat4 ToMatrix4();
	inline void Orthonormalise();
	inline bool Sensible() const;
	inline vec3 GetCol(unsigned int i) const;	
	inline void SetCol(unsigned int i, const vec3 & col);
	float m[9];
};
mat3 FIRECUBE_API Transpose(const mat3 mat);
mat3 FIRECUBE_API operator*(const mat3 &a,const float &b);
mat3 FIRECUBE_API operator*(const float &a,const mat3 &b);
vec3 FIRECUBE_API operator*(const mat3 &a,const vec3 &b);
vec3 FIRECUBE_API operator*(const vec3 &a,const mat3 &b);

#endif