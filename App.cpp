#include "glew.h"
#include "App.h"
#include "GLFrame.h"
#include "Controller.h"
#include "ControlFrame.h"

IMPLEMENT_APP(App)

bool App::OnInit() {
	_frame = new GLFrame( "Hello World", wxPoint(50, 50), wxSize(1024, 768) );
	_frame->Show( true );
	initShader("vir.json");
	initUniforms();
	initModels();
	ControlFrame* controlFrame = new ControlFrame();
	controlFrame->Show( true );
	_eventTimer = new wxTimer(this, kEventTimerId);
	_eventTimer->Start(1, wxTIMER_CONTINUOUS);
	return true;
}

int App::OnExit() {
	return 1;
}

void App::OnEventTimer(wxTimerEvent& event) {
	_frame->_canvas->Refresh();
}


BEGIN_EVENT_TABLE(App, wxApp)
EVT_TIMER(kEventTimerId, App::OnEventTimer)
END_EVENT_TABLE()