#include "MainFrameImpl.h"
#include <wx/msgdlg.h>

MainFrameImpl::MainFrameImpl( wxWindow* parent )
:
MainFrame( parent )
{

}

void MainFrameImpl::MyButtonClicked( wxCommandEvent& event )
{
	wxMessageBox("dsads");
}
