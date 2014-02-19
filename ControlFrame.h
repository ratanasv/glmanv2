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
	wxTextCtrl* _valueString;
	wxSizer* _sizer;
	wxSlider* _slider;
	wxStaticText* _text;
	void OnScroll(wxScrollEvent& event);
};