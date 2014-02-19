#pragma once
#include <wx/wxprec.h>
#include "GLCanvas.h"

class GLFrame: public wxFrame {
public:
	GLFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
	void OnHello(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void GLFrame::OnCanvasMotion(wxMouseEvent& event);
public:
	GLCanvas* _canvas;
};
