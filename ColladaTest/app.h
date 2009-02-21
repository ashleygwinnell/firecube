class App : public Application
{
public:
	bool Init();
	void Render(float time);
	void Update(float time);
	void HandleInput(float time);
	bool LoadCollada(const string &filename);
	void LoadMaterials();
	void LoadMesh(string &id,Model model,map<string,Material> &materialMap);
	void LoadNodes();
	Material GetMaterialByName(string &name);
	DAE colladaDom;	
	Font font;	
	Model model;
};