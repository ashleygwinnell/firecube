class App : public Application, public InputListener
{
public:
	App();
	bool Prepare();
	void HandleInput(float time, const MappedInput &input);
	void Update(float time);
	void Render(float time);

private:
	Terrain *terrain;
	NodePtr root, node;	
	Scene scene;
	NodePtr terrainNode;
	FontFacePtr fontFace;	
	CameraPtr orthographicCamera;
	CameraPtr camera;
	vec3 speed;
	vec3 angSpeed;
	vec3 ang;
};