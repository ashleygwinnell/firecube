#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

class FIRECUBE_API BoundingBox
{
public:
	BoundingBox();
	BoundingBox(const vec3 &min,const vec3 &max);
	void Expand(const BoundingBox &bb);
	void Expand(const vec3 &v);
	float GetWidth();
	float GetHeight();
	float GetDepth();
	vec3 GetCenter();
	bool Contains(const BoundingBox &bb);
	vec3 bmin,bmax;
};

#endif