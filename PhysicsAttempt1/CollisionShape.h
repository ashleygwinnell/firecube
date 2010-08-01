float PointTriangleDistanceSquared(vec3 pos,vec3 v0,vec3 v1,vec3 v2,float &S,float &T);
class PointInfo
{
public:
	vec3 dir;
	float dist;
};
class CollisionShape
{
public:
	CollisionShape();
	void FromNode(Node node,int sizex,int sizey,int sizez,float extraSize);
	bool GetPointInfo(vec3 pos, vec3 &dir, float &dist, bool accurateOutsideDist);
	Geometry GetFirstGeometry(Node node);
	vector<vec3> vertex;
	vector<Face> face;
	vector<Edge> edge;
	Grid3D<PointInfo> volume;
	vec3 min,max;
	float radius;
	vec3 dv;
	int sizex,sizey,sizez;
	bool interpolateMesh;
};