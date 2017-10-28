#pragma once

#include "../mainframe.h"
#include "PanelCommon.h"

class BaseComponentPanelImpl;
class ParticleEmitterDescriptor;

class ParticleEmitterPanelImpl : public ParticleEmitterPanel, public PanelCommon<ParticleEmitterDescriptor>
{
	FIRECUBE_OBJECT(ParticleEmitterPanelImpl);
public:
	ParticleEmitterPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine);
	~ParticleEmitterPanelImpl();
private:
	virtual void UpdateUI() override;
	void UpdateVisibility(FireCube::ParticleEmitterShape shape);
	virtual void ShapeTypeChanged(wxCommandEvent& event) override;
	virtual void MaterialFileChanged(wxFileDirPickerEvent& event) override;	
	virtual void PrewarmChanged(wxCommandEvent& event) override;
	virtual void SimulationSpaceChanged(wxCommandEvent& event) override;	

	BaseComponentPanelImpl *parent;	
};