///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __MAINFRAME_H__
#define __MAINFRAME_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/treectrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <queue>
#include <FireCube.h>
#include <wx/glcanvas.h>
#include "GlCanvas.h"
#include <wx/scrolwin.h>
#include <wx/frame.h>
#include <wx/aui/aui.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/statline.h>
#include <wx/filepicker.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/valtext.h>
#include <wx/choice.h>
#include <wx/clrpicker.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/listctrl.h>
#include <wx/statbmp.h>
#include <wx/splitter.h>
#include <wx/toolbar.h>
#include <wx/stc/stc.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* menuBar;
		wxMenu* fileMenu;
		wxMenu* editMenu;
		wxMenuItem* undoMenuItem;
		wxMenuItem* redoMenuItem;
		wxMenu* addMenu;
		wxMenu* m_menu1;
		wxMenu* viewMenu;
		wxMenuItem* viewSceneHierarchyMenuItem;
		wxMenuItem* viewInspectorMenuItem;
		wxMenuItem* viewMaterialEditorMenuItem;
		wxMenuItem* viewAssetBrowserMenuItem;
		wxPanel* m_panel1;
		wxTreeCtrl* sceneTreeCtrl;
		wxPanel* m_panel2;
		GLCanvas *glCanvas;
		wxPanel* inspectorPanel;
		wxScrolledWindow* componentsList;
		wxBoxSizer* componentsSizer;
		
		// Virtual event handlers, overide them in your derived class
		virtual void PaneClose( wxAuiManagerEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void NewClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void OpenClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void SaveClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void SaveAsClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExitClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void UndoClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void RedoClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddNodeClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddMeshClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddStaticModelClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddLightClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddLuaScriptClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddCollisionShapeClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddCharacterControllerClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddBoxClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddRigidBodyClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddPlaneClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddSphereClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void ViewSceneHierarchyClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void ViewInspectorClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void ViewMaterialEditorClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void ViewAssetBrowserClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void SceneTreeKeyUp( wxKeyEvent& event ) { event.Skip(); }
		virtual void SceneTreeBeginDrag( wxTreeEvent& event ) { event.Skip(); }
		virtual void SceneTreeEndDrag( wxTreeEvent& event ) { event.Skip(); }
		virtual void SceneTreeEndLabelEdit( wxTreeEvent& event ) { event.Skip(); }
		virtual void SceneTreeSelectionChanged( wxTreeEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Scene Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 941,706 ), long style = wxDEFAULT_FRAME_STYLE|wxMAXIMIZE|wxTAB_TRAVERSAL );
		wxAuiManager m_mgr;
		
		~MainFrame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class BaseComponentPanel
///////////////////////////////////////////////////////////////////////////////
class BaseComponentPanel : public wxPanel 
{
	private:
	
	protected:
		wxStaticText* componentTypeLabel;
		wxButton* removeComponent;
		wxBoxSizer* customComponentsSizer;
		wxStaticLine* m_staticline1;
		
		// Virtual event handlers, overide them in your derived class
		virtual void RemoveComponentClicked( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		BaseComponentPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL ); 
		~BaseComponentPanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class StaticModelPanel
///////////////////////////////////////////////////////////////////////////////
class StaticModelPanel : public wxPanel 
{
	private:
	
	protected:
		wxFilePickerCtrl* meshFilePicker;
		wxCheckBox* castShadowCheckBox;
		wxStaticText* m_staticText42;
		wxTextCtrl* lightMaskTextCtrl;
		wxStaticText* m_staticText421;
		wxTextCtrl* collisionQueryMaskTextCtrl;
		
		// Virtual event handlers, overide them in your derived class
		virtual void FileChanged( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void CastShadowChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void LightMaskChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void CollisionQueryMaskChanged( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxString lightMaskText; 
		wxString collisionQueryMaskText; 
		
		StaticModelPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL ); 
		~StaticModelPanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class BoxPanel
///////////////////////////////////////////////////////////////////////////////
class BoxPanel : public wxPanel 
{
	private:
	
	protected:
		wxTextCtrl* widthTextCtrl;
		wxTextCtrl* heightTextCtrl;
		wxTextCtrl* depthTextCtrl;
		wxStaticText* m_staticText48;
		wxFilePickerCtrl* materialFilePicker;
		wxCheckBox* castShadowCheckBox;
		wxStaticText* m_staticText42;
		wxTextCtrl* lightMaskTextCtrl;
		wxStaticText* m_staticText421;
		wxTextCtrl* collisionQueryMaskTextCtrl;
		
		// Virtual event handlers, overide them in your derived class
		virtual void WidthChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void HeightChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void DepthChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void MaterialFileChanged( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void CastShadowChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void LightMaskChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void CollisionQueryMaskChanged( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxString widthText; 
		wxString heightText; 
		wxString depthText; 
		wxString lightMaskText; 
		wxString collisionQueryMaskText; 
		
		BoxPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL ); 
		~BoxPanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class SpherePanel
///////////////////////////////////////////////////////////////////////////////
class SpherePanel : public wxPanel 
{
	private:
	
	protected:
		wxTextCtrl* radiusTextCtrl;
		wxTextCtrl* columnsTextCtrl;
		wxTextCtrl* ringsTextCtrl;
		wxStaticText* m_staticText48;
		wxFilePickerCtrl* materialFilePicker;
		wxCheckBox* castShadowCheckBox;
		wxStaticText* m_staticText42;
		wxTextCtrl* lightMaskTextCtrl;
		wxStaticText* m_staticText421;
		wxTextCtrl* collisionQueryMaskTextCtrl;
		
		// Virtual event handlers, overide them in your derived class
		virtual void RadiusChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void ColumnsChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void RingsChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void MaterialFileChanged( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void CastShadowChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void LightMaskChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void CollisionQueryMaskChanged( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxString radiusText; 
		wxString columnsText; 
		wxString ringsText; 
		wxString lightMaskText; 
		wxString collisionQueryMaskText; 
		
		SpherePanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL ); 
		~SpherePanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class PlanePanel
///////////////////////////////////////////////////////////////////////////////
class PlanePanel : public wxPanel 
{
	private:
	
	protected:
		wxTextCtrl* widthTextCtrl;
		wxTextCtrl* depthTextCtrl;
		wxStaticText* m_staticText48;
		wxFilePickerCtrl* materialFilePicker;
		wxCheckBox* castShadowCheckBox;
		wxStaticText* m_staticText42;
		wxTextCtrl* lightMaskTextCtrl;
		wxStaticText* m_staticText421;
		wxTextCtrl* collisionQueryMaskTextCtrl;
		
		// Virtual event handlers, overide them in your derived class
		virtual void WidthChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void DepthChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void MaterialFileChanged( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void CastShadowChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void LightMaskChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void CollisionQueryMaskChanged( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxString widthText; 
		wxString depthText; 
		wxString lightMaskText; 
		wxString collisionQueryMaskText; 
		
		PlanePanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL ); 
		~PlanePanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class RigidBodyPanel
///////////////////////////////////////////////////////////////////////////////
class RigidBodyPanel : public wxPanel 
{
	private:
	
	protected:
		wxTextCtrl* massTextCtrl;
		
		// Virtual event handlers, overide them in your derived class
		virtual void MassChanged( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxString massText; 
		
		RigidBodyPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL ); 
		~RigidBodyPanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class NodePropertiesPanel
///////////////////////////////////////////////////////////////////////////////
class NodePropertiesPanel : public wxPanel 
{
	private:
	
	protected:
		wxStaticText* m_staticText27;
		wxTextCtrl* nameTextCtrl;
		wxStaticText* m_staticText3;
		wxStaticText* m_staticText4;
		wxTextCtrl* positionXTextCtrl;
		wxStaticText* m_staticText5;
		wxTextCtrl* positionYTextCtrl;
		wxStaticText* m_staticText6;
		wxTextCtrl* positionZTextCtrl;
		wxStaticText* m_staticText31;
		wxStaticText* m_staticText41;
		wxTextCtrl* rotationXTextCtrl;
		wxStaticText* m_staticText51;
		wxTextCtrl* rotationYTextCtrl;
		wxStaticText* m_staticText61;
		wxTextCtrl* rotationZTextCtrl;
		wxStaticText* m_staticText32;
		wxStaticText* m_staticText42;
		wxTextCtrl* scaleXTextCtrl;
		wxStaticText* m_staticText52;
		wxTextCtrl* scaleYTextCtrl;
		wxStaticText* m_staticText62;
		wxTextCtrl* scaleZTextCtrl;
		wxStaticLine* m_staticline3;
		
		// Virtual event handlers, overide them in your derived class
		virtual void NameChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void PositionXChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void PositionYChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void PositionZChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void RotationXChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void RotationYChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void RotationZChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void ScaleXChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void ScaleYChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void ScaleZChanged( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxString nodePositionXText; 
		wxString nodePositionYText; 
		wxString nodePositionZText; 
		wxString nodeRotationXText; 
		wxString nodeRotationYText; 
		wxString nodeRotationZText; 
		wxString nodeScaleXText; 
		wxString nodeScaleYText; 
		wxString nodeScaleZText; 
		
		NodePropertiesPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 357,-1 ), long style = wxTAB_TRAVERSAL ); 
		~NodePropertiesPanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class LightPanel
///////////////////////////////////////////////////////////////////////////////
class LightPanel : public wxPanel 
{
	private:
	
	protected:
		wxStaticText* m_staticText16;
		wxChoice* lightTypeChoice;
		wxStaticText* m_staticText17;
		wxColourPickerCtrl* lightColorPicker;
		wxStaticText* m_staticText41;
		wxTextCtrl* maskTextCtrl;
		wxCheckBox* castShadowCheckBox;
		wxStaticText* m_staticText19;
		wxTextCtrl* shadowIntensityTextCtrl;
		wxPanel* rangePanel;
		wxStaticText* m_staticText21;
		wxTextCtrl* rangeTextCtrl;
		wxPanel* spotCutoffPanel;
		wxStaticText* m_staticText211;
		wxTextCtrl* spotCutoffTextCtrl;
		
		// Virtual event handlers, overide them in your derived class
		virtual void LightTypeChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void LightColorChanged( wxColourPickerEvent& event ) { event.Skip(); }
		virtual void MaskChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void CastShadowChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void ShadowIntensityChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void RangeChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void SpotCutoffChanged( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxString maskText; 
		wxString shadowIntensityText; 
		wxString rangeText; 
		wxString spotCutoffText; 
		
		LightPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL ); 
		~LightPanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class LuaScriptPanel
///////////////////////////////////////////////////////////////////////////////
class LuaScriptPanel : public wxPanel 
{
	private:
	
	protected:
		wxFilePickerCtrl* scriptFilePicker;
		wxTextCtrl* objectNameTextCtrl;
		wxButton* addProperty;
		wxButton* removeProperty;
		wxButton* renameProperty;
		wxPropertyGrid* propertyGrid;
		
		// Virtual event handlers, overide them in your derived class
		virtual void FileChanged( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void ObjectNameChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddPropertyClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void RemovePropertyClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void RenamePropertyClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void PropertyGridChanged( wxPropertyGridEvent& event ) { event.Skip(); }
		
	
	public:
		
		LuaScriptPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL ); 
		~LuaScriptPanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class CollisionShapePanel
///////////////////////////////////////////////////////////////////////////////
class CollisionShapePanel : public wxPanel 
{
	private:
	
	protected:
		wxStaticText* m_staticText25;
		wxChoice* shapeTypeChoice;
		wxCheckBox* triggerCheckBox;
		wxStaticText* planeStaticText;
		wxPanel* planePanel;
		wxStaticText* m_staticText27;
		wxTextCtrl* planeXTextCtrl;
		wxStaticText* m_staticText28;
		wxTextCtrl* planeYTextCtrl;
		wxStaticText* m_staticText29;
		wxTextCtrl* planeZTextCtrl;
		wxStaticText* m_staticText30;
		wxTextCtrl* planeWTextCtrl;
		wxStaticText* boxStaticText;
		wxPanel* boxPanel;
		wxStaticText* m_staticText271;
		wxTextCtrl* bboxMinXTextCtrl;
		wxStaticText* m_staticText281;
		wxTextCtrl* bboxMinYTextCtrl;
		wxStaticText* m_staticText292;
		wxTextCtrl* bboxMinZTextCtrl;
		wxStaticText* m_staticText272;
		wxTextCtrl* bboxMaxXTextCtrl;
		wxStaticText* m_staticText282;
		wxTextCtrl* bboxMaxYTextCtrl;
		wxStaticText* m_staticText293;
		wxTextCtrl* bboxMaxZTextCtrl;
		wxStaticText* meshStaticText;
		wxFilePickerCtrl* meshFilePicker;
		wxStaticText* sphereStaticText;
		wxTextCtrl* radiusTextCtrl;
		
		// Virtual event handlers, overide them in your derived class
		virtual void ShapeTypeChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void TriggerChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void PlaneXChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void PlaneYChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void PlaneZChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void PlaneWChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void BBoxMinXChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void BBoxMinYChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void BBoxMinZChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void BBoxMaxXChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void BBoxMaxYChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void BBoxMaxZChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void MeshFileChanged( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void RadiusChanged( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxString planeXText; 
		wxString planeYText; 
		wxString planeZText; 
		wxString planeWText; 
		wxString bboxMinXText; 
		wxString bboxMinYText; 
		wxString bboxMinZText; 
		wxString bboxMaxXText; 
		wxString bboxMaxYText; 
		wxString bboxMaxZText; 
		wxString radiusText; 
		
		CollisionShapePanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL ); 
		~CollisionShapePanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class CharacterControllerPanel
///////////////////////////////////////////////////////////////////////////////
class CharacterControllerPanel : public wxPanel 
{
	private:
	
	protected:
		wxStaticText* m_staticText3;
		wxTextCtrl* radiusTextCtrl;
		wxStaticText* m_staticText44;
		wxTextCtrl* heightTextCtrl;
		wxStaticText* m_staticText45;
		wxTextCtrl* contactOffsetTextCtrl;
		
		// Virtual event handlers, overide them in your derived class
		virtual void RadiusChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void HeightChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void ContactOffsetChanged( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxString radiusText; 
		wxString heightText; 
		
		CharacterControllerPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 357,-1 ), long style = wxTAB_TRAVERSAL ); 
		~CharacterControllerPanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class MaterialEditorPanel
///////////////////////////////////////////////////////////////////////////////
class MaterialEditorPanel : public wxPanel 
{
	private:
	
	protected:
		wxBoxSizer* bSizer37;
		wxBoxSizer* bSizer38;
		wxButton* newButton;
		wxButton* openButton;
		wxButton* saveButton;
		wxButton* saveAsButton;
		wxButton* pickMaterialButton;
		wxPropertyGrid* propertyGrid;
		
		// Virtual event handlers, overide them in your derived class
		virtual void NewButtonClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void OpenButtonClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void SaveButtonClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void SaveAsButtonClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void PickMaterialButtonClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void PropertyGridChanged( wxPropertyGridEvent& event ) { event.Skip(); }
		
	
	public:
		
		MaterialEditorPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 507,496 ), long style = wxTAB_TRAVERSAL ); 
		~MaterialEditorPanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class AssetBrowserPanel
///////////////////////////////////////////////////////////////////////////////
class AssetBrowserPanel : public wxPanel 
{
	private:
	
	protected:
		wxSplitterWindow* m_splitter1;
		wxPanel* m_panel8;
		wxTreeCtrl* directoryTreeCtrl;
		wxPanel* m_panel9;
		wxSplitterWindow* splitter2;
		wxPanel* m_panel10;
		wxListCtrl* fileListCtrl;
		wxPanel* m_panel11;
		wxStaticBitmap* texturePreviewBitmap;
		
		// Virtual event handlers, overide them in your derived class
		virtual void DirectoryTreeSelectionChanged( wxTreeEvent& event ) { event.Skip(); }
		virtual void FileListBeginDrag( wxListEvent& event ) { event.Skip(); }
		virtual void FileListItemActivated( wxListEvent& event ) { event.Skip(); }
		virtual void FileListItemSelected( wxListEvent& event ) { event.Skip(); }
		virtual void FileListKeyDown( wxListEvent& event ) { event.Skip(); }
		virtual void TexturePreviewBitmapResize( wxSizeEvent& event ) { event.Skip(); }
		
	
	public:
		
		AssetBrowserPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 507,496 ), long style = wxTAB_TRAVERSAL ); 
		~AssetBrowserPanel();
		
		void m_splitter1OnIdle( wxIdleEvent& )
		{
			m_splitter1->SetSashPosition( 150 );
			m_splitter1->Disconnect( wxEVT_IDLE, wxIdleEventHandler( AssetBrowserPanel::m_splitter1OnIdle ), NULL, this );
		}
		
		void splitter2OnIdle( wxIdleEvent& )
		{
			splitter2->SetSashPosition( 2000 );
			splitter2->Disconnect( wxEVT_IDLE, wxIdleEventHandler( AssetBrowserPanel::splitter2OnIdle ), NULL, this );
		}
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ScriptEditorPanel
///////////////////////////////////////////////////////////////////////////////
class ScriptEditorPanel : public wxPanel 
{
	private:
	
	protected:
		wxToolBar* m_toolBar1;
		wxToolBarToolBase* saveTool; 
		wxStyledTextCtrl* sourceText;
	
	public:
		
		ScriptEditorPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 773,746 ), long style = wxTAB_TRAVERSAL ); 
		~ScriptEditorPanel();
	
};

#endif //__MAINFRAME_H__
