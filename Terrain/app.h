class App : public Application, public InputListener
{
public:
	App();
	bool Init();
	void HandleInput(float time, const MappedInput &input);
	void Update(float time);
	void Render(float time);

private:
	NodePtr root, node;	
	TerrainNodePtr terrainNode;
	FontPtr font;	
	CameraPtr orthographicCamera;
	CameraPtr camera;
	vec3 speed;
	vec3 angSpeed;
	vec3 ang;
};