#pragma once

#include "../mainframe.h"

class BaseComponentPanelImpl;
class Command;

class ParticleEmitterPanelImpl : public ParticleEmitterPanel, public FireCube::Object
{
	FIRECUBE_OBJECT(ParticleEmitterPanelImpl);
public:
	ParticleEmitterPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine);
	~ParticleEmitterPanelImpl();
private:
	void UpdateUI();
	void UpdateVisibility(FireCube::ParticleEmitterShape shape);
	virtual void ShapeTypeChanged(wxCommandEvent& event) override;
	virtual void MaterialFileChanged(wxFileDirPickerEvent& event) override;	
	virtual void PrewarmChanged(wxCommandEvent& event) override;
	virtual void SimulationSpaceChanged(wxCommandEvent& event) override;
	void UndoPerformed(Command *command);

	BaseComponentPanelImpl *parent;
	FireCube::vec3 prevBox;
	float prevFloatVal;
	unsigned int prevUIntVal;
	FireCube::vec2 prevVec2Val;
	Command *prevCommand;
};