#pragma once

#include "../mainframe.h"

class BaseComponentPanelImpl;

class ParticleEmitterPanelImpl : public ParticleEmitterPanel
{
public:
	ParticleEmitterPanelImpl(BaseComponentPanelImpl* parent);
	~ParticleEmitterPanelImpl();
private:
	void UpdateUI();
	void UpdateVisibility(FireCube::ParticleEmitterShape shape);
	virtual void ShapeTypeChanged(wxCommandEvent& event);
	virtual void BBoxWidthChanged(wxCommandEvent& event);
	virtual void BBoxHeightChanged(wxCommandEvent& event);
	virtual void BBoxDepthChanged(wxCommandEvent& event);
	virtual void RadiusChanged(wxCommandEvent& event);
	virtual void NumberOfParticlesChanged(wxCommandEvent& event);
	virtual void EmissionRateChanged(wxCommandEvent& event);
	virtual void MaterialFileChanged(wxFileDirPickerEvent& event);
	virtual void LifeTimeChanged(wxCommandEvent& event);

	BaseComponentPanelImpl *parent;
};