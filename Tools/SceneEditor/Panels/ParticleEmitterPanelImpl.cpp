#include "BaseComponentPanelImpl.h"
#include "ParticleEmitterPanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../Descriptors/ParticleEmitterDescriptor.h"
#include "../AssetUtils.h"

using namespace FireCube;

ParticleEmitterPanelImpl::ParticleEmitterPanelImpl(BaseComponentPanelImpl* parent) : ParticleEmitterPanel(parent), parent(parent)
{
	ParticleEmitterDescriptor *particleEmitter = static_cast<ParticleEmitterDescriptor *>(parent->GetComponent());
	
	UpdateUI();
	UpdateVisibility(particleEmitter->GetEmitterShape());
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

	radiusTextCtrl->SetLabelText(wxString::FromDouble(particleEmitter->GetRadius()));
	bboxWidthTextCtrl->SetLabelText(wxString::FromDouble(particleEmitter->GetBox().x));
	bboxHeightTextCtrl->SetLabelText(wxString::FromDouble(particleEmitter->GetBox().y));
	bboxDepthTextCtrl->SetLabelText(wxString::FromDouble(particleEmitter->GetBox().z));
	numberOfParticlesTextCtrl->SetLabelText(wxString::Format(wxT("%i"), particleEmitter->GetNumberOfParticles()));
	emissionRateTextCtrl->SetLabelText(wxString::Format(wxT("%i"), particleEmitter->GetEmissionRate()));
	minLifeTimeTextCtrl->SetLabelText(wxString::FromDouble(particleEmitter->GetMinLifeTime()));
	maxLifeTimeTextCtrl->SetLabelText(wxString::FromDouble(particleEmitter->GetMaxLifeTime()));
	minSpeedTextCtrl->SetLabelText(wxString::FromDouble(particleEmitter->GetMinSpeed()));
	maxSpeedTextCtrl->SetLabelText(wxString::FromDouble(particleEmitter->GetMaxSpeed()));
	materialFilePicker->SetPath(particleEmitter->GetMaterial());	
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

	UpdateVisibility(newShape);

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
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void ParticleEmitterPanelImpl::BBoxWidthChanged(wxCommandEvent& event)
{
	ParticleEmitterDescriptor *particleEmitter = static_cast<ParticleEmitterDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	double x, y, z;
	event.GetString().ToDouble(&x);
	bboxHeightTextCtrl->GetValue().ToDouble(&y);
	bboxDepthTextCtrl->GetValue().ToDouble(&z);	
	vec3 oldBox = particleEmitter->GetBox();
	vec3 newBox(x, y, z);

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Box", [particleEmitter, newBox]()
	{
		particleEmitter->SetBoxEmitter(newBox);
	}, [particleEmitter, oldBox]()
	{
		particleEmitter->SetBoxEmitter(oldBox);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void ParticleEmitterPanelImpl::BBoxHeightChanged(wxCommandEvent& event)
{
	ParticleEmitterDescriptor *particleEmitter = static_cast<ParticleEmitterDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	double x, y, z;
	event.GetString().ToDouble(&y);
	bboxWidthTextCtrl->GetValue().ToDouble(&x);	
	bboxDepthTextCtrl->GetValue().ToDouble(&z);
	vec3 oldBox = particleEmitter->GetBox();
	vec3 newBox(x, y, z);

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Box", [particleEmitter, newBox]()
	{
		particleEmitter->SetBoxEmitter(newBox);
	}, [particleEmitter, oldBox]()
	{
		particleEmitter->SetBoxEmitter(oldBox);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void ParticleEmitterPanelImpl::BBoxDepthChanged(wxCommandEvent& event)
{
	ParticleEmitterDescriptor *particleEmitter = static_cast<ParticleEmitterDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	double x, y, z;
	event.GetString().ToDouble(&z);
	bboxWidthTextCtrl->GetValue().ToDouble(&x);
	bboxHeightTextCtrl->GetValue().ToDouble(&y);
	vec3 oldBox = particleEmitter->GetBox();
	vec3 newBox(x, y, z);

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Box", [particleEmitter, newBox]()
	{
		particleEmitter->SetBoxEmitter(newBox);
	}, [particleEmitter, oldBox]()
	{
		particleEmitter->SetBoxEmitter(oldBox);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void ParticleEmitterPanelImpl::RadiusChanged(wxCommandEvent& event)
{
	ParticleEmitterDescriptor *particleEmitter = static_cast<ParticleEmitterDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	double newRadius;
	event.GetString().ToDouble(&newRadius);
	double oldRadius = particleEmitter->GetRadius();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Radius", [particleEmitter, newRadius]()
	{
		particleEmitter->SetSphereEmitter(newRadius);
	}, [particleEmitter, oldRadius]()
	{
		particleEmitter->SetSphereEmitter(oldRadius);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void ParticleEmitterPanelImpl::NumberOfParticlesChanged(wxCommandEvent& event)
{
	ParticleEmitterDescriptor *particleEmitter = static_cast<ParticleEmitterDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	long newNumberOfParticles;
	event.GetString().ToLong(&newNumberOfParticles);
	long oldNumberOfParticles = particleEmitter->GetNumberOfParticles();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Number Of Particles", [particleEmitter, newNumberOfParticles]()
	{
		particleEmitter->SetNumberOfParticles(newNumberOfParticles);
	}, [particleEmitter, oldNumberOfParticles]()
	{
		particleEmitter->SetNumberOfParticles(oldNumberOfParticles);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void ParticleEmitterPanelImpl::EmissionRateChanged(wxCommandEvent& event)
{
	ParticleEmitterDescriptor *particleEmitter = static_cast<ParticleEmitterDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	long newEmissionRate;
	event.GetString().ToLong(&newEmissionRate);
	long oldEmissionRate = particleEmitter->GetEmissionRate();	

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Emission Rate", [particleEmitter, newEmissionRate]()
	{
		particleEmitter->SetEmissionRate(newEmissionRate);
	}, [particleEmitter, oldEmissionRate]()
		{
			particleEmitter->SetEmissionRate(oldEmissionRate);
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
	}, [particleEmitter, oldValue]()
	{
		particleEmitter->SetMaterial(oldValue);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void ParticleEmitterPanelImpl::MinLifeTimeChanged(wxCommandEvent& event)
{
	ParticleEmitterDescriptor *particleEmitter = static_cast<ParticleEmitterDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	double newMinLifeTime;
	double maxLifeTime = particleEmitter->GetMaxLifeTime();
	event.GetString().ToDouble(&newMinLifeTime);
	double oldMinLifeTime = particleEmitter->GetMinLifeTime();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Life Time", [particleEmitter, newMinLifeTime, maxLifeTime]()
	{
		particleEmitter->SetLifeTime(newMinLifeTime, maxLifeTime);
	}, [particleEmitter, oldMinLifeTime, maxLifeTime]()
	{
		particleEmitter->SetLifeTime(oldMinLifeTime, maxLifeTime);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void ParticleEmitterPanelImpl::MaxLifeTimeChanged(wxCommandEvent& event)
{
	ParticleEmitterDescriptor *particleEmitter = static_cast<ParticleEmitterDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	double newMaxLifeTime;
	double minLifeTime = particleEmitter->GetMinLifeTime();
	event.GetString().ToDouble(&newMaxLifeTime);
	double oldMaxLifeTime = particleEmitter->GetMinLifeTime();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Life Time", [particleEmitter, minLifeTime, newMaxLifeTime]()
	{
		particleEmitter->SetLifeTime(minLifeTime, newMaxLifeTime);
	}, [particleEmitter, minLifeTime, oldMaxLifeTime]()
	{
		particleEmitter->SetLifeTime(minLifeTime, oldMaxLifeTime);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void ParticleEmitterPanelImpl::MinSpeedChanged(wxCommandEvent& event)
{
	ParticleEmitterDescriptor *particleEmitter = static_cast<ParticleEmitterDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	double newMinSpeed;
	double maxSpeed = particleEmitter->GetMaxSpeed();
	event.GetString().ToDouble(&newMinSpeed);
	double oldMinSpeed = particleEmitter->GetMinSpeed();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Speed", [particleEmitter, newMinSpeed, maxSpeed]()
	{
		particleEmitter->SetSpeed(newMinSpeed, maxSpeed);
	}, [particleEmitter, oldMinSpeed, maxSpeed]()
	{
		particleEmitter->SetSpeed(oldMinSpeed, maxSpeed);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void ParticleEmitterPanelImpl::MaxSpeedChanged(wxCommandEvent& event)
{
	ParticleEmitterDescriptor *particleEmitter = static_cast<ParticleEmitterDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	double newMaxSpeed;
	double minSpeed = particleEmitter->GetMinSpeed();
	event.GetString().ToDouble(&newMaxSpeed);
	double oldMaxSpeed = particleEmitter->GetMaxSpeed();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Speed", [particleEmitter, minSpeed, newMaxSpeed]()
	{
		particleEmitter->SetSpeed(minSpeed, newMaxSpeed);
	}, [particleEmitter, minSpeed, oldMaxSpeed]()
	{
		particleEmitter->SetSpeed(minSpeed, oldMaxSpeed);
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
	}, [particleEmitter, oldPrewarm]()
	{
		particleEmitter->SetPrewarm(oldPrewarm);
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
	}, [particleEmitter, oldSimulationSpace]()
	{
		particleEmitter->SetSimulationSpace(oldSimulationSpace);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}
