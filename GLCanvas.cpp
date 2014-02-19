#include <glew.h>
#include "GLCanvas.h"
#include <wx/dcclient.h>
#include <wx/wxprec.h>
#include "glslprogram.h"
#include <memory>
#include "Controller.h"
#include "Model.h"

using namespace std;

#ifdef MACOSX
#pragma mark GLCanvas
#endif

void Draw(const int width, const int height) {
	glReadBuffer(GL_FRONT);
	glDrawBuffer( GL_BACK );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );

	// set the viewport to a square centered in the window:
	glViewport(0.0, 0.0, width, height);

	m_shader->Use();
	for (auto it : m_Uniforms) {
		m_shader->SetUniform(it.second._name, it.second._value);
	}

	m_shader->ClearModelView();
	m_shader->LookAt( vec3<>(0., 0., 15.), vec3<>(0., 0., 0.), vec3<>(0., 1., 0.));

	m_shader->Translate(0.0, 0.0, m_zTranslate);
	m_shader->Rotate(m_xRot, m_yRot, 0.0);

	m_shader->ClearProjection();
	m_shader->Perspective( 90., 1.,	0.1, 100. );

	glBegin(GL_TRIANGLES);
		glVertex3f(-1.0, -1.0, 0.0);
		glVertex3f(1.0, -1.0, 0.0);
		glVertex3f(0.0, 1.0, 0.0);
	glEnd();
}


GLCanvas::GLCanvas(wxWindow* parent, wxWindowID id, int* attribList) : 
	wxGLCanvas(parent, id, wxDefaultPosition, wxDefaultSize, 0, _T("GLCanvas"), attribList)
{

	SetCurrent();
	SwapBuffers();
	if (glewInit() != GLEW_OK) {
		wxMessageBox(_T("GLEW init failure"),
			_T("MessageCriticalError"), wxOK|wxICON_ERROR  );
	}
	

	Connect(wxEVT_PAINT, wxPaintEventHandler(GLCanvas::OnPaint), NULL, this);
	Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(GLCanvas::OnEraseBackground), NULL, this);

	SetFocus();
}

void	GLCanvas::OnPaint(wxPaintEvent& event) {
	//SetFocus();
	wxSize size(GetSize());
	wxPaintDC(this);

	SetCurrent(*GetContext());
	Draw(size.GetWidth(), size.GetHeight());
	SwapBuffers();
}

void	GLCanvas::OnEraseBackground(wxEraseEvent& event) {
	// Ignore this event to prevent flickering on Windows
}