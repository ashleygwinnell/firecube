class App : public Application, public InputListener
{
public:
	App();
	~App();
    bool Prepare();
    void Update(float time);
    void Render(float time);
    void HandleInput(float time, const MappedInput &input);

    FontFace *fontFace;
    Node *root, *node, *node2;
	Node *mainRoot;
    FrameBuffer *fbo;
    Camera *camera, *camera2;	
	Scene scene, scene2;
	Material *material;
	Geometry *plane;
};