class App : public Application, public InputListener
{
public:
	App();	
    bool Prepare();
    void Update(float time);
    void Render(float time);
    void HandleInput(float time, const MappedInput &input);

	UIText *text;
    Node *node, *node2;    
	SharedPtr<RenderSurface> renderSurface;
    Camera *camera, *camera2;	
	Scene scene, scene2;
	SharedPtr<Material> material;	
};