#include "ControlFrame.h"
#include "Controller.h"
#include "Model.h"
#include "vir_toolbox.hpp"

using namespace std;

ControlFrame::ControlFrame() : 
	wxFrame(NULL, wxID_ANY, "Controllers", wxPoint(1200, 50), wxSize(400, 500))
{
	wxSizer* bigSizer = new wxBoxSizer(wxVERTICAL);

	for (auto uniform : m_Uniforms) {
		SliderWithID* slider = new SliderWithID(this, uniform.second._name);
		bigSizer->Add(slider, 1, wxEXPAND);
	}
	
	SetSizer(bigSizer);
}

static float ToImaginary(const string& id, const float real) {
	const float min = m_Uniforms[id]._minValue;
	const float max = m_Uniforms[id]._maxValue;
	return (real-min)/(max-min)*100.0;
}

static float ToReal(const string& id, const float imaginary) {
	const float min = m_Uniforms[id]._minValue;
	const float max = m_Uniforms[id]._maxValue;
	return (imaginary/100.0)*(max-min) + min;
}

SliderWithID::SliderWithID(wxWindow* parent, const string& id) : 
	wxPanel(parent, wxID_ANY), _id(id)
{
	const float defaultValue = ToImaginary(_id, m_Uniforms[_id]._value);
	_slider = new wxSlider(this, wxID_ANY, defaultValue, 0, 100);
	_sizer = new wxBoxSizer(wxHORIZONTAL);

	_value = new wxTextCtrl(this, wxID_ANY, m_Uniforms[_id]._name, 
		wxDefaultPosition, wxSize(40, 20), wxTE_PROCESS_ENTER);

	_sizer->Add(_slider, 2, wxEXPAND);
	_sizer->Add(_value, 1);

	_slider->Connect(wxEVT_SCROLL_THUMBTRACK , 
		wxScrollEventHandler(SliderWithID::OnScroll), NULL, this);

	SetSizer(_sizer);
}

void SliderWithID::OnScroll(wxScrollEvent& event) {
	m_Uniforms[_id]._value = ToReal(_id, _slider->GetValue());
	_value->SetValue(vvprintf("%f", m_Uniforms[_id]._value));
}
