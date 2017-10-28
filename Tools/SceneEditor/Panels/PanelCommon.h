#pragma once

#include <FireCube.h>
#include "EventBindingHelpers.h"
#include "../app.h"
#include <wx/textctrl.h>

class Command;

template <class Desc>
class PanelCommon : public FireCube::Object
{
	FIRECUBE_OBJECT(PanelCommon);
public:
	PanelCommon(FireCube::Engine *engine, Desc *component) : Object(engine), component(component), prevCommand(nullptr), skipUiUpdate(false)
	{
		MyApp *theApp = ((MyApp *)wxTheApp);
		editorState = theApp->GetEditorState();
		SubscribeToEvent(theApp->GetEditorState(), theApp->GetEditorState()->undoPerformed, &PanelCommon::UndoPerformed);
		SubscribeToEvent(component->componentChanged, &PanelCommon<Desc>::UpdateUIInternal);
	}
protected:	

	// unsigned int helper functions
	void BindTextCtrlUInt(wxTextCtrl *textCtrl, const std::string &description, std::function<unsigned int(Desc *)> getValue, std::function<void(Desc *, const unsigned int &)> setValue, std::function<unsigned int(Desc *, wxCommandEvent &)> getNewValue)
	{
		EventBindingHelpers::BindTextCtrl<unsigned int, Desc>(textCtrl, component, engine, editorState, description, getValue, setValue, [this, getNewValue](Desc *desc, wxCommandEvent &evt) {
			skipUiUpdate = true;
			return getNewValue(desc, evt);
		}, prevCommand, prevUInt);
	}

	void BindTextCtrl(wxTextCtrl *textCtrl, const std::string &description, unsigned int (Desc::*getter)() const, void (Desc::*setter)(unsigned int))
	{
		BindTextCtrlUInt(textCtrl, description, [getter](Desc *component) -> unsigned int {
			return (component->*getter)();
		}, [setter](Desc *component, const unsigned int &newVal) {
			(component->*setter)(newVal);
		}, [](Desc *desc, wxCommandEvent &evt) -> unsigned int {			
			unsigned long newVal;
			evt.GetString().ToULong(&newVal);
			return (unsigned int)newVal;
		});
	}

	void BindTextCtrl(wxTextCtrl *textCtrl, const std::string &description, unsigned int (Desc::*getter)() const, void (Desc::*setter)(unsigned int, FireCube::Engine *))
	{
		auto engine = this->engine;
		BindTextCtrlUInt(textCtrl, description, [getter](Desc *component) -> unsigned int {
			return (component->*getter)();
		}, [setter, engine](Desc *component, const unsigned int &newVal) {
			(component->*setter)(newVal, engine);
		}, [](Desc *desc, wxCommandEvent &evt) -> unsigned int {
			unsigned long newVal;
			evt.GetString().ToULong(&newVal);
			return (unsigned int)newVal;
		});
	}

	void BindTextCtrlHex(wxTextCtrl *textCtrl, const std::string &description, unsigned int (Desc::*getter)() const, void (Desc::*setter)(unsigned int))
	{
		BindTextCtrlUInt(textCtrl, description, [getter](Desc *component) -> unsigned int {
			return (component->*getter)();
		}, [setter](Desc *component, const unsigned int &newVal) {
			(component->*setter)(newVal);
		}, [](Desc *desc, wxCommandEvent &evt) -> unsigned int {
			unsigned long newVal;
			evt.GetString().ToULong(&newVal, 16);
			return (unsigned int)newVal;
		});
	}

	// float helper functions
	void BindTextCtrlFloat(wxTextCtrl *textCtrl, const std::string &description, std::function<float(Desc *)> getValue, std::function<void(Desc *, const float &)> setValue, std::function<float(Desc *, wxCommandEvent &)> getNewValue)
	{
		EventBindingHelpers::BindTextCtrl<float, Desc>(textCtrl, component, engine, editorState, description, getValue, setValue, [this, getNewValue](Desc *desc, wxCommandEvent &evt) {
			skipUiUpdate = true;
			return getNewValue(desc, evt);
		}, prevCommand, prevFloat);
	}

	void BindTextCtrl(wxTextCtrl *textCtrl, const std::string &description, float (Desc::*getter)() const, void (Desc::*setter)(float))
	{
		BindTextCtrlFloat(textCtrl, description, [getter](Desc *component) -> float {
			return (component->*getter)();
		}, [setter](Desc *component, const float &newVal) {
			(component->*setter)(newVal);
		}, [](Desc *desc, wxCommandEvent &evt) -> float {
			double newVal;
			evt.GetString().ToDouble(&newVal);
			return (float)newVal;
		});
	}

	void BindTextCtrl(wxTextCtrl *textCtrl, const std::string &description, float (Desc::*getter)() const, void (Desc::*setter)(float, FireCube::Engine *))
	{
		auto engine = this->engine;
		BindTextCtrlFloat(textCtrl, description, [getter](Desc *component) -> float {
			return (component->*getter)();
		}, [setter, engine](Desc *component, const float &newVal) {
			(component->*setter)(newVal, engine);
		}, [](Desc *desc, wxCommandEvent &evt) -> float {
			double newVal;
			evt.GetString().ToDouble(&newVal);
			return (float)newVal;
		});
	}

	// vec2 helper functions
	void BindTextCtrlVec2(wxTextCtrl *textCtrl, const std::string &description, std::function<FireCube::vec2(Desc *)> getValue, std::function<void(Desc *, const FireCube::vec2 &)> setValue, std::function<FireCube::vec2(Desc *, wxCommandEvent &)> getNewValue)
	{
		EventBindingHelpers::BindTextCtrl<FireCube::vec2, Desc>(textCtrl, component, engine, editorState, description, getValue, setValue, [this, getNewValue](Desc *desc, wxCommandEvent &evt) {
			skipUiUpdate = true;
			return getNewValue(desc, evt);
		}, prevCommand, prevVec2);
	}

	void BindTextCtrl(wxTextCtrl *textCtrl0, const std::string &description0, wxTextCtrl *textCtrl1, const std::string &description1, FireCube::vec2(Desc::*getter)() const, void (Desc::*setter)(FireCube::vec2, FireCube::Engine *))
	{
		auto engine = this->engine;
		auto valueGetter = [getter](Desc *component) -> FireCube::vec2 {
			return (component->*getter)();
		};

		auto valueSetter = [setter, engine](Desc *component, const vec2 &v) {
			(component->*setter)(v, engine);
		};

		auto valueEvtHandler = [getter, textCtrl0, textCtrl1](Desc *component, wxCommandEvent &evt) -> vec2 {
			double newVal;
			evt.GetString().ToDouble(&newVal);
			vec2 oldValue = (component->*getter)();

			if (evt.GetEventObject() == textCtrl0)
			{
				return vec2(newVal, oldValue.y);
			}
			else if (evt.GetEventObject() == textCtrl1)
			{
				return vec2(oldValue.x, newVal);
			}

			return vec2(0);
		};


		BindTextCtrlVec2(textCtrl0, description0, valueGetter, valueSetter, valueEvtHandler);
		BindTextCtrlVec2(textCtrl1, description1, valueGetter, valueSetter, valueEvtHandler);		
	}
	
	// vec3 helper functions
	void BindTextCtrlVec3(wxTextCtrl *textCtrl, const std::string &description, std::function<FireCube::vec3(Desc *)> getValue, std::function<void(Desc *, const FireCube::vec3 &)> setValue, std::function<FireCube::vec3(Desc *, wxCommandEvent &)> getNewValue)
	{
		EventBindingHelpers::BindTextCtrl<FireCube::vec3, Desc>(textCtrl, component, engine, editorState, description, getValue, setValue, [this, getNewValue](Desc *desc, wxCommandEvent &evt) {
			skipUiUpdate = true;
			return getNewValue(desc, evt);
		}, prevCommand, prevVec3);
	}

	void BindTextCtrl(wxTextCtrl *textCtrl0, const std::string &description0, wxTextCtrl *textCtrl1, const std::string &description1, wxTextCtrl *textCtrl2, const std::string &description2, FireCube::vec3 (Desc::*getter)() const, void (Desc::*setter)(FireCube::vec3, FireCube::Engine *))
	{
		auto engine = this->engine;
		auto valueGetter = [getter](Desc *component) -> FireCube::vec3 {
			return (component->*getter)();
		};

		auto valueSetter = [setter, engine](Desc *component, const vec3 &v) {
			(component->*setter)(v, engine);
		};

		auto valueEvtHandler = [getter, textCtrl0, textCtrl1, textCtrl2](Desc *component, wxCommandEvent &evt) -> vec3 {
			double newVal;
			evt.GetString().ToDouble(&newVal);
			vec3 oldValue = (component->*getter)();

			if (evt.GetEventObject() == textCtrl0)
			{
				return vec3(newVal, oldValue.y, oldValue.z);
			}
			else if (evt.GetEventObject() == textCtrl1)
			{
				return vec3(oldValue.x, newVal, oldValue.z);
			}
			else if (evt.GetEventObject() == textCtrl2)
			{
				return vec3(oldValue.x, oldValue.y, newVal);
			}

			return vec3(0);
		};


		BindTextCtrlVec3(textCtrl0, description0, valueGetter, valueSetter, valueEvtHandler);
		BindTextCtrlVec3(textCtrl1, description1, valueGetter, valueSetter, valueEvtHandler);
		BindTextCtrlVec3(textCtrl2, description2, valueGetter, valueSetter, valueEvtHandler);
	}

	void BindTextCtrl(wxTextCtrl *textCtrl0, const std::string &description0, wxTextCtrl *textCtrl1, const std::string &description1, wxTextCtrl *textCtrl2, const std::string &description2, FireCube::vec3(Desc::*getter)() const, void (Desc::*setter)(FireCube::vec3))
	{	
		auto valueGetter = [getter](Desc *component) -> FireCube::vec3 {
			return (component->*getter)();
		};

		auto valueSetter = [setter](Desc *component, const vec3 &v) {
			(component->*setter)(v);
		};

		auto valueEvtHandler = [getter, textCtrl0, textCtrl1, textCtrl2](Desc *component, wxCommandEvent &evt) -> vec3 {
			double newVal;
			evt.GetString().ToDouble(&newVal);
			vec3 oldValue = (component->*getter)();

			if (evt.GetEventObject() == textCtrl0)
			{
				return vec3(newVal, oldValue.y, oldValue.z);
			}
			else if (evt.GetEventObject() == textCtrl1)
			{
				return vec3(oldValue.x, newVal, oldValue.z);
			}
			else if (evt.GetEventObject() == textCtrl2)
			{
				return vec3(oldValue.x, oldValue.y, newVal);
			}

			return vec3(0);
		};


		BindTextCtrlVec3(textCtrl0, description0, valueGetter, valueSetter, valueEvtHandler);
		BindTextCtrlVec3(textCtrl1, description1, valueGetter, valueSetter, valueEvtHandler);
		BindTextCtrlVec3(textCtrl2, description2, valueGetter, valueSetter, valueEvtHandler);
	}

	void BindTextCtrl(wxTextCtrl *textCtrl0, wxTextCtrl *textCtrl1, wxTextCtrl *textCtrl2, const std::string &description, FireCube::vec3(Desc::*getter)() const, void (Desc::*setter)(FireCube::vec3))
	{
		BindTextCtrl(textCtrl0, description, textCtrl1, description, textCtrl2, description, getter, setter);
	}

	// vec4 helper functions
	void BindTextCtrlVec4(wxTextCtrl *textCtrl, const std::string &description, std::function<FireCube::vec4(Desc *)> getValue, std::function<void(Desc *, const FireCube::vec4 &)> setValue, std::function<FireCube::vec4(Desc *, wxCommandEvent &)> getNewValue)
	{
		EventBindingHelpers::BindTextCtrl<FireCube::vec4, Desc>(textCtrl, component, engine, editorState, description, getValue, setValue, [this, getNewValue](Desc *desc, wxCommandEvent &evt) {
			skipUiUpdate = true;
			return getNewValue(desc, evt);
		}, prevCommand, prevVec4);
	}

	void BindTextCtrl(wxTextCtrl *textCtrl0, const std::string &description0, wxTextCtrl *textCtrl1, const std::string &description1, wxTextCtrl *textCtrl2, const std::string &description2, wxTextCtrl *textCtrl3, const std::string &description3, FireCube::vec4(Desc::*getter)() const, void (Desc::*setter)(FireCube::vec4, FireCube::Engine *))
	{
		auto engine = this->engine;
		auto valueGetter = [getter](Desc *component) -> FireCube::vec4 {
			return (component->*getter)();
		};

		auto valueSetter = [setter, engine](Desc *component, const vec4 &v) {
			(component->*setter)(v, engine);
		};

		auto valueEvtHandler = [getter, textCtrl0, textCtrl1, textCtrl2, textCtrl3](Desc *component, wxCommandEvent &evt) -> vec4 {
			double newVal;
			evt.GetString().ToDouble(&newVal);
			vec4 oldValue = (component->*getter)();

			if (evt.GetEventObject() == textCtrl0)
			{
				return vec4(newVal, oldValue.y, oldValue.z, oldValue.w);
			}
			else if (evt.GetEventObject() == textCtrl1)
			{
				return vec4(oldValue.x, newVal, oldValue.z, oldValue.w);
			}
			else if (evt.GetEventObject() == textCtrl2)
			{
				return vec4(oldValue.x, oldValue.y, newVal, oldValue.w);
			}
			else if (evt.GetEventObject() == textCtrl3)
			{
				return vec4(oldValue.x, oldValue.y, oldValue.z, newVal);
			}

			return vec4(0);
		};


		BindTextCtrlVec4(textCtrl0, description0, valueGetter, valueSetter, valueEvtHandler);
		BindTextCtrlVec4(textCtrl1, description1, valueGetter, valueSetter, valueEvtHandler);
		BindTextCtrlVec4(textCtrl2, description2, valueGetter, valueSetter, valueEvtHandler);
		BindTextCtrlVec4(textCtrl3, description3, valueGetter, valueSetter, valueEvtHandler);
	}

	// Plane helper functions
	void BindTextCtrlPlane(wxTextCtrl *textCtrl, const std::string &description, std::function<FireCube::Plane(Desc *)> getValue, std::function<void(Desc *, const FireCube::Plane &)> setValue, std::function<FireCube::Plane(Desc *, wxCommandEvent &)> getNewValue)
	{
		EventBindingHelpers::BindTextCtrl<FireCube::Plane, Desc>(textCtrl, component, engine, editorState, description, getValue, setValue, [this, getNewValue](Desc *desc, wxCommandEvent &evt) {
			skipUiUpdate = true;
			return getNewValue(desc, evt);
		}, prevCommand, prevPlane);
	}

	void BindTextCtrl(wxTextCtrl *textCtrl0, wxTextCtrl *textCtrl1, wxTextCtrl *textCtrl2, wxTextCtrl *textCtrl3, const std::string &description, FireCube::Plane(Desc::*getter)() const, void (Desc::*setter)(FireCube::Plane))
	{	
		auto valueGetter = [getter](Desc *component) -> FireCube::Plane {
			return (component->*getter)();
		};

		auto valueSetter = [setter](Desc *component, const FireCube::Plane &v) {
			(component->*setter)(v);
		};

		auto valueEvtHandler = [getter, textCtrl0, textCtrl1, textCtrl2, textCtrl3](Desc *component, wxCommandEvent &evt) -> FireCube::Plane {
			double newVal;
			evt.GetString().ToDouble(&newVal);

			Plane curPlane = (component->*getter)();
			if (evt.GetEventObject() == textCtrl0)
			{
				curPlane.SetNormal(vec3((float)newVal, curPlane.GetNormal().y, curPlane.GetNormal().z));
			}
			else if (evt.GetEventObject() == textCtrl1)
			{
				curPlane.SetNormal(vec3(curPlane.GetNormal().x, (float)newVal, curPlane.GetNormal().z));
			}
			else if (evt.GetEventObject() == textCtrl2)
			{
				curPlane.SetNormal(vec3(curPlane.GetNormal().x, curPlane.GetNormal().y, (float)newVal));
			}
			if (evt.GetEventObject() == textCtrl3)
			{
				curPlane.SetDistance(newVal);
			}

			return curPlane;
		};


		BindTextCtrlPlane(textCtrl0, description, valueGetter, valueSetter, valueEvtHandler);
		BindTextCtrlPlane(textCtrl1, description, valueGetter, valueSetter, valueEvtHandler);
		BindTextCtrlPlane(textCtrl2, description, valueGetter, valueSetter, valueEvtHandler);
		BindTextCtrlPlane(textCtrl3, description, valueGetter, valueSetter, valueEvtHandler);
	}

	// BoundingBox helper functions
	void BindTextCtrlBoundingBox(wxTextCtrl *textCtrl, const std::string &description, std::function<FireCube::BoundingBox(Desc *)> getValue, std::function<void(Desc *, const FireCube::BoundingBox &)> setValue, std::function<FireCube::BoundingBox(Desc *, wxCommandEvent &)> getNewValue)
	{
		EventBindingHelpers::BindTextCtrl<FireCube::BoundingBox, Desc>(textCtrl, component, engine, editorState, description, getValue, setValue, [this, getNewValue](Desc *desc, wxCommandEvent &evt) {
			skipUiUpdate = true;
			return getNewValue(desc, evt);
		}, prevCommand, prevBoundingBox);
	}

	void BindTextCtrl(wxTextCtrl *textCtrlMin0, wxTextCtrl *textCtrlMin1, wxTextCtrl *textCtrlMin2, wxTextCtrl *textCtrlMax0, wxTextCtrl *textCtrlMax1, wxTextCtrl *textCtrlMax2, const std::string &description, FireCube::BoundingBox(Desc::*getter)() const, void (Desc::*setter)(FireCube::BoundingBox))
	{
		auto valueGetter = [getter](Desc *component) -> FireCube::BoundingBox {
			return (component->*getter)();
		};

		auto valueSetter = [setter](Desc *component, const FireCube::BoundingBox &v) {
			(component->*setter)(v);
		};

		auto valueEvtHandler = [getter, textCtrlMin0, textCtrlMin1, textCtrlMin2, textCtrlMax0, textCtrlMax1, textCtrlMax2](Desc *component, wxCommandEvent &evt) -> FireCube::BoundingBox {
			double newVal;
			evt.GetString().ToDouble(&newVal);

			BoundingBox curBox = (component->*getter)();
			if (evt.GetEventObject() == textCtrlMin0)
			{
				curBox.SetMin(vec3((float)newVal, curBox.GetMin().y, curBox.GetMin().z));
			}
			else if (evt.GetEventObject() == textCtrlMin1)
			{
				curBox.SetMin(vec3(curBox.GetMin().x, (float)newVal, curBox.GetMin().z));
			}
			else if (evt.GetEventObject() == textCtrlMin2)
			{
				curBox.SetMin(vec3(curBox.GetMin().x, curBox.GetMin().y, (float)newVal));
			}

			else if (evt.GetEventObject() == textCtrlMax0)
			{
				curBox.SetMax(vec3((float)newVal, curBox.GetMax().y, curBox.GetMax().z));
			}
			else if (evt.GetEventObject() == textCtrlMax1)
			{
				curBox.SetMax(vec3(curBox.GetMax().x, (float)newVal, curBox.GetMax().z));
			}
			else if (evt.GetEventObject() == textCtrlMax2)
			{
				curBox.SetMax(vec3(curBox.GetMax().x, curBox.GetMax().y, (float)newVal));
			}

			return curBox;
		};


		BindTextCtrlBoundingBox(textCtrlMin0, description, valueGetter, valueSetter, valueEvtHandler);
		BindTextCtrlBoundingBox(textCtrlMin1, description, valueGetter, valueSetter, valueEvtHandler);
		BindTextCtrlBoundingBox(textCtrlMin2, description, valueGetter, valueSetter, valueEvtHandler);
		BindTextCtrlBoundingBox(textCtrlMax0, description, valueGetter, valueSetter, valueEvtHandler);
		BindTextCtrlBoundingBox(textCtrlMax1, description, valueGetter, valueSetter, valueEvtHandler);
		BindTextCtrlBoundingBox(textCtrlMax2, description, valueGetter, valueSetter, valueEvtHandler);
	}
protected:
	Desc *component;
	EditorState *editorState;

	void ChangeValue(wxTextCtrl *textCtrl, const wxString &value)
	{
		wxString prevValue = textCtrl->GetValue();
		if (prevValue != value)
		{
			textCtrl->ChangeValue(value);
			textCtrl->SetInsertionPointEnd();
		}
	}
private:
	virtual void UpdateUI() = 0;
	void UpdateUIInternal()
	{
		if (skipUiUpdate)
		{
			skipUiUpdate = false;
			return;
		}

		UpdateUI();
	}
	void UndoPerformed(Command *command)
	{
		prevCommand = nullptr;
	}
		
	unsigned int prevUInt;	
	float prevFloat;
	FireCube::vec2 prevVec2;
	FireCube::vec3 prevVec3;
	FireCube::vec4 prevVec4;
	FireCube::Plane prevPlane;
	FireCube::BoundingBox prevBoundingBox;
	Command *prevCommand;
	bool skipUiUpdate;	
};