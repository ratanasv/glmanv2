#pragma once 
#include <wx/glcanvas.h>

class GLCanvas: public wxGLCanvas {
private:
	static bool	s_is3DInitialized;


public:
	static wxGLContext* sharedGLContext;
	GLCanvas(wxWindow* parent, wxWindowID id, int* attribList = 0);

	void	OnPaint(wxPaintEvent& event);
	void	OnEraseBackground(wxEraseEvent& event);

};
