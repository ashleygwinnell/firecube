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
	Node *root, *cameraNode;	
	Scene scene;	
	FontFace *fontFace;		
	Camera *camera;
	vec3 speed;
	vec3 angSpeed;
	vec3 ang;
};