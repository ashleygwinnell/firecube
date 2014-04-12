class App : public Application, public InputListener
{
public:
	App();
	~App();
    void HandleInput(float t, const MappedInput &input);
    void Render(float t);
    void Update(float t);
    bool Prepare();
    FontFace *fontFace;
    Node *root;	
	Camera *camera;
	Geometry *lightMarker, *earthGeometry;
	Material *earthMaterial;
	Scene scene;
};