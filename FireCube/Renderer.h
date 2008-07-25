#ifndef RENDERER_H
#define RENDERER_H

#pragma warning(push)
#pragma warning(disable:4251)

class FIRECUBE_API ShaderResource
{
public:
	ShaderResource();
	~ShaderResource();
	bool Load(const string &filename);
	GLuint id;
};

typedef boost::shared_ptr<ShaderResource> Shader;
typedef ResourceManager<ShaderResource> ShaderManager;

class FIRECUBE_API Program
{
public:
	Program();
	~Program();
	void Create();
	void Create(Shader shader1,Shader shader2);
	void Attach(Shader shader);
	void Link();
	void Use(bool useShaders=true);
	void Uniform1f(const string &name,float value);
	void Uniform1i(const string &name,int value);	
	GLuint id;
	map<string,GLint> variables;
};
class FIRECUBE_API Renderer
{
public:
	Renderer();
	~Renderer();
	void Clear();

	void Render(Model model);
	void SetModelViewMatrix(mat4 &m);
	void SetProjectionMatrix(mat4 &m);
	void SaveModelViewMatrix();
	void RestoreModelViewMatrix();
	void MultiplyModelViewMatrix(mat4 &mat);
private:	
};


#pragma warning(pop)

#endif