class ColladaLoader
{
public:
	class Source
	{
	public:
		vector <float> floatArray;
		int stride;
	};
	ColladaLoader(ModelResource *model);
	bool Load(const string &filename);
private:	
	Material LoadMaterial(const string &name,map<string,pair<string,DWORD>> &vertexInput);
	void LoadMesh(string &id,mat4 transform, map<string,string> &materialMap,map<string,pair<string,DWORD>> &vertexInput);
	void LoadNodes();
	void LoadImageMap();
	vec4 ReadColor(daeElement *elm);
	bool ReadTexture(daeElement *elm,map<string,pair<string,DWORD>> &vertexInput,string &file,DWORD &unit);
	mat4 ReadTransformation(domNode *node);
	Source ReadSource(domSourceRef source);
	vector<vec3> FloatArray2Vec3Array(const vector<float> &fa,int stride);
	vector<vec2> FloatArray2Vec2Array(const vector<float> &fa,int stride);
	Material GetMaterialByName(string &name);	
	DAE colladaDom;
	ModelResource *model;
	map<string,string> imageMap;
};