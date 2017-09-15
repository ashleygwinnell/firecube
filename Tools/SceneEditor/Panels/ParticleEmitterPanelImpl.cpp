#include "BaseComponentPanelImpl.h"
#include "ParticleEmitterPanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../Descriptors/ParticleEmitterDescriptor.h"
#include "../AssetUtils.h"
#include "EventBindingHelpers.h"

using namespace FireCube;

ParticleEmitterPanelImpl::ParticleEmitterPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine) : ParticleEmitterPanel(parent), parent(parent), Object(engine)
{
	MyApp *theApp = ((MyApp *)wxTheApp);
	ParticleEmitterDescriptor *particleEmitter = static_cast<ParticleEmitterDescriptor *>(parent->GetComponent());
	
	UpdateUI();
	
	SubscribeToEvent(particleEmitter->componentChanged, &ParticleEmitterPanelImpl::UpdateUI);
	SubscribeToEvent(theApp->GetEditorState(), theApp->GetEditorState()->undoPerformed, &ParticleEmitterPanelImpl::UndoPerformed);

	EventBindingHelpers::BindTextCtrl<float, ParticleEmitterDescriptor>(radiusTextCtrl, particleEmitter, engine, theApp->GetEditorState(), "Change Radius", [](ParticleEmitterDescriptor *particleEmitter) {
		return particleEmitter->GetRadius();
	}, [](ParticleEmitterDescriptor *particleEmitter, const float &radius) {
		particleEmitter->SetSphereEmitter(radius);
	}, [](ParticleEmitterDescriptor *particleEmitter, wxCommandEvent &evt) {
		double newVal;
		evt.GetString().ToDouble(&newVal);
		return (float)newVal;
	}, prevCommand, prevFloatVal);

	auto boxGetter = [](ParticleEmitterDescriptor *particleEmitter) {
		return particleEmitter->GetBox();
	};

	auto boxSetter = [](ParticleEmitterDescriptor *particleEmitter, const vec3 &box) {
		return particleEmitter->SetBoxEmitter(box);
	};

	auto boxEvtHandler = [this](ParticleEmitterDescriptor *particleEmitter, wxCommandEvent &evt) {
		double newVal;
		evt.GetString().ToDouble(&newVal);
		vec3 curBox = particleEmitter->GetBox();
		if (evt.GetEventObject() == bboxWidthTextCtrl)
		{
			curBox.x = newVal;
		}
		else if (evt.GetEventObject() == bboxHeightTextCtrl)
		{
			curBox.y = newVal;
		}
		else if (evt.GetEventObject() == bboxDepthTextCtrl)
		{
			curBox.z = newVal;
		}

		return curBox;
	};

	EventBindingHelpers::BindTextCtrl<vec3, ParticleEmitterDescriptor>(bboxWidthTextCtrl, particleEmitter, engine, theApp->GetEditorState(), "Change Box", boxGetter, boxSetter, boxEvtHandler, prevCommand, prevBox);
	EventBindingHelpers::BindTextCtrl<vec3, ParticleEmitterDescriptor>(bboxHeightTextCtrl, particleEmitter, engine, theApp->GetEditorState(), "Change Box", boxGetter, boxSetter, boxEvtHandler, prevCommand, prevBox);
	EventBindingHelpers::BindTextCtrl<vec3, ParticleEmitterDescriptor>(bboxDepthTextCtrl, particleEmitter, engine, theApp->GetEditorState(), "Change Box", boxGetter, boxSetter, boxEvtHandler, prevCommand, prevBox);

	EventBindingHelpers::BindTextCtrl<unsigned int, ParticleEmitterDescriptor>(numberOfParticlesTextCtrl, particleEmitter, engine, theApp->GetEditorState(), "Change Number Of Particles", [](ParticleEmitterDescriptor *particleEmitter) {
		return particleEmitter->GetNumberOfParticles();
	}, [](ParticleEmitterDescriptor *particleEmitter, const unsigned int &newVal) {
		particleEmitter->SetNumberOfParticles(newVal);
	}, [](ParticleEmitterDescriptor *particleEmitter, wxCommandEvent &evt) {
		unsigned long newVal;
		evt.GetString().ToULong(&newVal);
		return (unsigned int)newVal;
	}, prevCommand, prevUIntVal);

	EventBindingHelpers::BindTextCtrl<unsigned int, ParticleEmitterDescriptor>(emissionRateTextCtrl, particleEmitter, engine, theApp->GetEditorState(), "Change Emission Rate", [](ParticleEmitterDescriptor *particleEmitter) {
		return particleEmitter->GetEmissionRate();
	}, [](ParticleEmitterDescriptor *particleEmitter, const unsigned int &newVal) {
		particleEmitter->SetEmissionRate(newVal);
	}, [](ParticleEmitterDescriptor *particleEmitter, wxCommandEvent &evt) {
		unsigned long newVal;
		evt.GetString().ToULong(&newVal);
		return (unsigned int)newVal;
	}, prevCommand, prevUIntVal);

	auto lifeTimeGetter = [](ParticleEmitterDescriptor *particleEmitter) {
		return vec2(particleEmitter->GetMinLifeTime(), particleEmitter->GetMaxLifeTime());
	};

	auto lifeTimeSetter = [](ParticleEmitterDescriptor *particleEmitter, const vec2 &newVal) {
		return particleEmitter->SetLifeTime(newVal.x, newVal.y);
	};

	auto lifeTimeEvtHandler = [this](ParticleEmitterDescriptor *particleEmitter, wxCommandEvent &evt) {
		double newVal;
		evt.GetString().ToDouble(&newVal);
		vec2 curLifeTime = vec2(particleEmitter->GetMinLifeTime(), particleEmitter->GetMaxLifeTime());
		if (evt.GetEventObject() == minLifeTimeTextCtrl)
		{
			curLifeTime.x = newVal;
		}
		else if (evt.GetEventObject() == maxLifeTimeTextCtrl)
		{
			curLifeTime.y = newVal;
		}		

		return curLifeTime;
	};

	EventBindingHelpers::BindTextCtrl<vec2, ParticleEmitterDescriptor>(minLifeTimeTextCtrl, particleEmitter, engine, theApp->GetEditorState(), "Change Life Time", lifeTimeGetter, lifeTimeSetter, lifeTimeEvtHandler, prevCommand, prevVec2Val);
	EventBindingHelpers::BindTextCtrl<vec2, ParticleEmitterDescriptor>(maxLifeTimeTextCtrl, particleEmitter, engine, theApp->GetEditorState(), "Change Life Time", lifeTimeGetter, lifeTimeSetter, lifeTimeEvtHandler, prevCommand, prevVec2Val);

	auto speedGetter = [](ParticleEmitterDescriptor *particleEmitter) {
		return vec2(particleEmitter->GetMinSpeed(), particleEmitter->GetMaxSpeed());
	};

	auto speedSetter = [](ParticleEmitterDescriptor *particleEmitter, const vec2 &newVal) {
		return particleEmitter->SetSpeed(newVal.x, newVal.y);
	};

	auto speedEvtHandler = [this](ParticleEmitterDescriptor *particleEmitter, wxCommandEvent &evt) {
		double newVal;
		evt.GetString().ToDouble(&newVal);
		vec2 curSpeed = vec2(particleEmitter->GetMinSpeed(), particleEmitter->GetMaxSpeed());
		if (evt.GetEventObject() == minSpeedTextCtrl)
		{
			curSpeed.x = newVal;
		}
		else if (evt.GetEventObject() == maxSpeedTextCtrl)
		{
			curSpeed.y = newVal;
		}

		return curSpeed;
	};

	EventBindingHelpers::BindTextCtrl<vec2, ParticleEmitterDescriptor>(minSpeedTextCtrl, particleEmitter, engine, theApp->GetEditorState(), "Change Speed", speedGetter, speedSetter, speedEvtHandler, prevCommand, prevVec2Val);
	EventBindingHelpers::BindTextCtrl<vec2, ParticleEmitterDescriptor>(maxSpeedTextCtrl, particleEmitter, engine, theApp->GetEditorState(), "Change Speed", speedGetter, speedSetter, speedEvtHandler, prevCommand, prevVec2Val);
}

ParticleEmitterPanelImpl::~ParticleEmitterPanelImpl()
{

}

void ParticleEmitterPanelImpl::UpdateUI()
{
	ParticleEmitterDescriptor *particleEmitter = static_cast<ParticleEmitterDescriptor *>(parent->GetComponent());

	ParticleEmitterShape shape = particleEmitter->GetEmitterShape();
	switch (shape)
	{
	case ParticleEmitterShape::BOX:
		shapeTypeChoice->SetSelection(0);
		break;	
	case ParticleEmitterShape::SPHERE:
		shapeTypeChoice->SetSelection(1);
		break;
	default:
		break;
	}

	UpdateVisibility(shape);

	switch (particleEmitter->GetSimulationSpace())
	{
	case ParticleEmitterSimulationSpace::LOCAL:
		simulationSpaceChoice->SetSelection(0);
		break;
	case ParticleEmitterSimulationSpace::WORLD:
		simulationSpaceChoice->SetSelection(1);
		break;
	default:
		break;
	}

	radiusTextCtrl->ChangeValue(wxString::FromDouble(particleEmitter->GetRadius()));
	bboxWidthTextCtrl->ChangeValue(wxString::FromDouble(particleEmitter->GetBox().x));
	bboxHeightTextCtrl->ChangeValue(wxString::FromDouble(particleEmitter->GetBox().y));
	bboxDepthTextCtrl->ChangeValue(wxString::FromDouble(particleEmitter->GetBox().z));
	numberOfParticlesTextCtrl->ChangeValue(wxString::Format(wxT("%i"), particleEmitter->GetNumberOfParticles()));
	emissionRateTextCtrl->ChangeValue(wxString::Format(wxT("%i"), particleEmitter->GetEmissionRate()));
	minLifeTimeTextCtrl->ChangeValue(wxString::FromDouble(particleEmitter->GetMinLifeTime()));
	maxLifeTimeTextCtrl->ChangeValue(wxString::FromDouble(particleEmitter->GetMaxLifeTime()));
	minSpeedTextCtrl->ChangeValue(wxString::FromDouble(particleEmitter->GetMinSpeed()));
	maxSpeedTextCtrl->ChangeValue(wxString::FromDouble(particleEmitter->GetMaxSpeed()));
	materialFilePicker->SetPath(particleEmitter->GetMaterial());	
	materialFilePicker->SetInitialDirectory(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Materials");
	prewarmCheckBox->SetValue(particleEmitter->GetPrewarm());
}

void ParticleEmitterPanelImpl::UpdateVisibility(ParticleEmitterShape shape)
{
	parent->GetParent()->Freeze();

	boxStaticText->Hide();
	sphereStaticText->Hide();
	boxPanel->Hide();
	radiusTextCtrl->Hide();

	switch (shape)
	{	
	case ParticleEmitterShape::BOX:
		boxStaticText->Show();
		boxPanel->Show();
		break;
	case ParticleEmitterShape::SPHERE:
		sphereStaticText->Show();
		radiusTextCtrl->Show();
		break;
	default:
		break;
	}

	this->Fit();

	parent->Fit();

	parent->GetParent()->FitInside();
	parent->GetParent()->Layout();
	parent->GetParent()->Thaw();
}

void ParticleEmitterPanelImpl::ShapeTypeChanged(wxCommandEvent& event)
{
	ParticleEmitterDescriptor *particleEmitter = static_cast<ParticleEmitterDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);	

	ParticleEmitterShape newShape;
	switch (shapeTypeChoice->GetSelection())
	{
	case 0:
		newShape = ParticleEmitterShape::BOX;
		break;
	case 1:
		newShape = ParticleEmitterShape::SPHERE;
		break;
	default:
		break;
	}	

	ParticleEmitterShape oldShape = particleEmitter->GetEmitterShape();
	vec3 oldBox = particleEmitter->GetBox();
	float oldRadius = particleEmitter->GetRadius();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Shape Type", [particleEmitter, newShape, oldBox, oldRadius]()
	{
		switch (newShape)
		{
		case ParticleEmitterShape::BOX:
			particleEmitter->SetBoxEmitter(oldBox);
			break;
		case ParticleEmitterShape::SPHERE:
			particleEmitter->SetSphereEmitter(oldRadius);
			break;		
		default:
			break;
		}
		particleEmitter->componentChanged(nullptr);
	}, [particleEmitter, oldShape, oldBox, oldRadius]()
	{
		switch (oldShape)
		{
		case ParticleEmitterShape::BOX:
			particleEmitter->SetBoxEmitter(oldBox);
			break;
		case ParticleEmitterShape::SPHERE:
			particleEmitter->SetSphereEmitter(oldRadius);
			break;
		default:
			break;
		}
		particleEmitter->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void ParticleEmitterPanelImpl::MaterialFileChanged(wxFileDirPickerEvent& event)
{
	ParticleEmitterDescriptor *particleEmitter = static_cast<ParticleEmitterDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	
	std::string newValue = AssetUtils::ImportMaterialIfNeeded(event.GetPath().ToStdString());
	std::string oldValue = particleEmitter->GetMaterial();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Material", [particleEmitter, newValue]()
	{
		particleEmitter->SetMaterial(newValue);
		particleEmitter->componentChanged(nullptr);
	}, [particleEmitter, oldValue]()
	{
		particleEmitter->SetMaterial(oldValue);
		particleEmitter->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void ParticleEmitterPanelImpl::PrewarmChanged(wxCommandEvent& event)
{
	ParticleEmitterDescriptor *particleEmitter = static_cast<ParticleEmitterDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	bool oldPrewarm = particleEmitter->GetPrewarm();
	bool newPrewarm = event.IsChecked();
	
	auto command = new CustomCommand(theApp->GetEditorState(), "Change Prewarm", [particleEmitter, newPrewarm]()
	{
		particleEmitter->SetPrewarm(newPrewarm);
		particleEmitter->componentChanged(nullptr);
	}, [particleEmitter, oldPrewarm]()
	{
		particleEmitter->SetPrewarm(oldPrewarm);
		particleEmitter->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void ParticleEmitterPanelImpl::SimulationSpaceChanged(wxCommandEvent& event)
{
	ParticleEmitterDescriptor *particleEmitter = static_cast<ParticleEmitterDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	ParticleEmitterSimulationSpace oldSimulationSpace = particleEmitter->GetSimulationSpace();
	ParticleEmitterSimulationSpace newSimulationSpace;
	switch (event.GetSelection())
	{
	case 0:
		newSimulationSpace = ParticleEmitterSimulationSpace::LOCAL;
		break;
	case 1:
		newSimulationSpace = ParticleEmitterSimulationSpace::WORLD;
		break;
	default:
		break;
	}

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Simulation Space", [particleEmitter, newSimulationSpace]()
	{
		particleEmitter->SetSimulationSpace(newSimulationSpace);
		particleEmitter->componentChanged(nullptr);
	}, [particleEmitter, oldSimulationSpace]()
	{
		particleEmitter->SetSimulationSpace(oldSimulationSpace);
		particleEmitter->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void ParticleEmitterPanelImpl::UndoPerformed(Command *command)
{
	prevCommand = nullptr;
}
