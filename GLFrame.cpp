#include "GLFrame.h"
#include "GLCanvas.h"
#include "Model.h"

GLFrame::GLFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	Show(true);
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(wxID_ANY, "&Hello...\tCtrl-H",
		"Help string shown in status bar for this menu item");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);
	wxMenu* menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append( menuFile, "&File" );
	menuBar->Append( menuHelp, "&Help" );
	SetMenuBar( menuBar );
	CreateStatusBar();
	SetStatusText( "Welcome to wxWidgets!" );

	int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 32, 0};
	wxBoxSizer* contentSizer = new wxBoxSizer(wxVERTICAL);
	_canvas = new GLCanvas(this, wxID_ANY, args);
	contentSizer->Add(_canvas, 1, wxEXPAND);

	_canvas->Connect(wxEVT_MOTION, wxMouseEventHandler(GLFrame::OnCanvasMotion), NULL, this);
}


void GLFrame::OnCanvasMotion(wxMouseEvent& event) {
	const float ANGFACT = 1.0;
	const float SCLFACT = 0.005;
	int dx = event.m_x - m_xMouse;		// change in mouse coords
	int dy = event.m_y - m_yMouse;
	if (event.LeftIsDown()) {
		m_xRot += ( ANGFACT*dy );
		m_xRot += ( ANGFACT*dx );


	} else if (event.MiddleIsDown()) {
		m_zTranslate +=  SCLFACT*dy;
	}
	else {
		
	}
	m_xMouse = event.m_x;
	m_yMouse = event.m_y;
	event.Skip(true);
	_canvas->Refresh();
}