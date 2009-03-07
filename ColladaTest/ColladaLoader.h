class ColladaLoader
{
public:
	class Source
	{
	public:
		vector <float> floatArray;
		DWORD stride;
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
	vector<vec4> SourceToVecArray(const Source &source);	
	Material GetMaterialByName(string &name);	
	DAE colladaDom;
	ModelResource *model;
	map<string,string> imageMap;
	string basepath;
};