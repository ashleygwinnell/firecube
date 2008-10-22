class Plane
{
public:
	float GetDist(vec3 p);
	vec3 normal;
	float d;
};

class Frustum
{
public:
	void ExtractFrustum();
	bool CubeInFrustum(vec3 pos,float size);
	bool BoxInFrustum(vec3 pos,vec3 size);

	Plane plane[6];
};