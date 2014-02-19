#pragma once
#include <wx/wxprec.h>
#include <vector>
#include <string>

class Uniform;

class ControlFrame : public wxFrame {
public:
	ControlFrame();
};

class SliderWithID : public wxPanel {
public:
	SliderWithID(wxWindow* parent, const std::string& id);
	const std::string _id;
	wxTextCtrl* _value;
	wxSizer* _sizer;
	wxSlider* _slider;
	void OnScroll(wxScrollEvent& event);
};