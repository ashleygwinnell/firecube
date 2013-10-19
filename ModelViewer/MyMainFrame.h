#ifndef __MyMainFrame__
#define __MyMainFrame__

/**
@file
Subclass of MainFrame, which is generated by wxFormBuilder.
*/

#include "mainframe.h"

class MyApp;
/** Implementing MainFrame */
class MyMainFrame : public MainFrame
{
protected:
    // Handlers for MainFrame events.
    void MenuItem1Clicked( wxCommandEvent& event );
    void MenuItem2Clicked( wxCommandEvent& event );    
    void CheckBox2Clicked( wxCommandEvent& event );
    void CheckBox4Clicked( wxCommandEvent& event );
    void RadioBox1Clicked( wxCommandEvent& event );
    void TextCtrl3TextEnter( wxCommandEvent& event );
    void CheckBox3Clicked( wxCommandEvent& event );
    void ColourPicker1Changed( wxColourPickerEvent& event );
    void Button1Clicked( wxCommandEvent& event );
    void Button2Clicked( wxCommandEvent& event );
    void TextCtrl4TextEnter( wxCommandEvent& event );
    void TextCtrl5TextEnter( wxCommandEvent& event );
    void MenuItem5Clicked( wxCommandEvent& event );
    void MenuItem6Clicked( wxCommandEvent& event );    
    void PropertyGrid1Changed( wxCommandEvent& event );
	bool AddMaterial(DWORD id, FireCube::MaterialPtr mat);
	void PopulateTreeWithNode(FireCube::Node *node, wxTreeCtrl *tree);
	void PopulateTreeWithNode(FireCube::Node *node, wxTreeItemId treeNode, wxTreeCtrl *tree);
	void AddMaterials(const std::vector<FireCube::MaterialPtr> &materials);
	MyApp *theApp;

public:
    /** Constructor */
    MyMainFrame( wxWindow* parent );
    void UpdateUI(Document &document);
	void SetStatusBarText(const std::string &text);
	GLCanvas *GetMainGLCanvas();
	class TreeItemData : public wxTreeItemData
	{
	public:
		FireCube::Node *node;
	};
    std::map<DWORD, FireCube::MaterialPtr> materialMap;
};

#endif // __MyMainFrame__
