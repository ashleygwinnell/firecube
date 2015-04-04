class App : public Application
{
public:
	App();
	~App();
    void HandleInput(float t, const MappedInput &input);
    void Render(float t);
    void Update(float t);
    bool Prepare();
	UIText *text;
    Node *root;	
	Camera *camera;
	Mesh *lightMarker, *earthMesh;	
	Scene scene;
};