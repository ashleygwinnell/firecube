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

ParticleEmitterPanelImpl::ParticleEmitterPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine) : ParticleEmitterPanel(parent), parent(parent), 
	PanelCommon(engine, static_cast<ParticleEmitterDescriptor *>(parent->GetComponent()))
{
	UpdateUI();
		
	BindTextCtrl(radiusTextCtrl, "Change Radius", &ParticleEmitterDescriptor::GetRadius, &ParticleEmitterDescriptor::SetSphereEmitter);
	BindTextCtrl(bboxWidthTextCtrl, bboxHeightTextCtrl, bboxDepthTextCtrl, "Change Box", &ParticleEmitterDescriptor::GetBox, &ParticleEmitterDescriptor::SetBoxEmitter);	
	BindTextCtrl(numberOfParticlesTextCtrl, "Change Number Of Particles", &ParticleEmitterDescriptor::GetNumberOfParticles, &ParticleEmitterDescriptor::SetNumberOfParticles);
	BindTextCtrl(emissionRateTextCtrl, "Change Emission Rate", &ParticleEmitterDescriptor::GetEmissionRate, &ParticleEmitterDescriptor::SetEmissionRate);
	BindTextCtrl(minLifeTimeTextCtrl, "Change Life Time", &ParticleEmitterDescriptor::GetMinLifeTime, &ParticleEmitterDescriptor::SetMinLifeTime);
	BindTextCtrl(maxLifeTimeTextCtrl, "Change Life Time", &ParticleEmitterDescriptor::GetMaxLifeTime, &ParticleEmitterDescriptor::SetMaxLifeTime);	
	BindTextCtrl(minSpeedTextCtrl, "Change Speed", &ParticleEmitterDescriptor::GetMinSpeed, &ParticleEmitterDescriptor::SetMinSpeed);
	BindTextCtrl(maxSpeedTextCtrl, "Change Speed", &ParticleEmitterDescriptor::GetMaxSpeed, &ParticleEmitterDescriptor::SetMaxSpeed);
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

	ChangeValue(radiusTextCtrl, wxString::FromDouble(particleEmitter->GetRadius()));
	ChangeValue(bboxWidthTextCtrl, wxString::FromDouble(particleEmitter->GetBox().x));
	ChangeValue(bboxHeightTextCtrl, wxString::FromDouble(particleEmitter->GetBox().y));
	ChangeValue(bboxDepthTextCtrl, wxString::FromDouble(particleEmitter->GetBox().z));
	ChangeValue(numberOfParticlesTextCtrl, wxString::Format(wxT("%i"), particleEmitter->GetNumberOfParticles()));
	ChangeValue(emissionRateTextCtrl, wxString::Format(wxT("%i"), particleEmitter->GetEmissionRate()));
	ChangeValue(minLifeTimeTextCtrl, wxString::FromDouble(particleEmitter->GetMinLifeTime()));
	ChangeValue(maxLifeTimeTextCtrl, wxString::FromDouble(particleEmitter->GetMaxLifeTime()));
	ChangeValue(minSpeedTextCtrl, wxString::FromDouble(particleEmitter->GetMinSpeed()));
	ChangeValue(maxSpeedTextCtrl, wxString::FromDouble(particleEmitter->GetMaxSpeed()));
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