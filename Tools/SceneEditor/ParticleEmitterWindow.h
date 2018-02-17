#pragma once

#include "FireCube.h"
#include "ImguiHelpers.h"
#include "imguifilesystem.h"

class NodeDescriptor;
class EditorState;
class ParticleEmitterDescriptor;

class ParticleEmitterWindow : public FireCube::Object
{
	FIRECUBE_OBJECT(ParticleEmitterWindow);
public:
	ParticleEmitterWindow(FireCube::Engine *engine);
	void Render(EditorState *editorState, ParticleEmitterDescriptor *descriptor);
private:
	Vec3InputHelper sizeInput;
	FloatInputHelper radiusInput;
	UIntInputHelper particleCountInput;
	UIntInputHelper emissionRateInput;
	UIntInputHelper ringsInput;
	CheckBoxHelper prewarmCheckBox;
	Vec2InputHelper lifeTimeInput;
	Vec2InputHelper speedInput;
	
};