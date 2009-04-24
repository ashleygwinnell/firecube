class MyApp : public wxApp
{
public:
	virtual bool OnInit();
	virtual int OnExit();
	MyMainFrame *frame;
};