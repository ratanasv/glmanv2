#pragma once;
#include <wx/wxprec.h>

#define kEventTimerId 1001

class GLFrame;

class App: public wxApp {
public:
	//Overridden Methods
	virtual bool	OnInit();
	virtual int		OnExit();
	virtual void	OnEventTimer(wxTimerEvent& event);

	DECLARE_EVENT_TABLE();
private:
	wxTimer* _eventTimer;
	GLFrame* _frame;
};

