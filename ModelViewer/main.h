class FireCubeApp : public FireCube::Application
{
public:
	void HandleInput(float t);
	void Render(float t);
	void Update(float t);
	void LoadModel(const string &filename);
	void GenerateNormals(FireCube::Node node,float l,vector<FireCube::vec3> &normals);
	void GenerateNormals(float l);
	void AddMaterials(unsigned int &id,FireCube::Node node);
	void CountElements(FireCube::Node node,unsigned int &verticesCount,unsigned int &faceCount);
	FireCube::Node root;
	FireCube::Font font;
	FireCube::Shader vshader,fshader;
	FireCube::Program program;	
	FireCube::Buffer normalRenderingBuffer;
	int normalRenderingBufferSize;
	FireCube::Program plainColorProgram;
};

class MyApp : public wxApp
{
public:
	virtual bool OnInit();
	MyMainFrame *frame;
	FireCubeApp fireCubeApp;
};