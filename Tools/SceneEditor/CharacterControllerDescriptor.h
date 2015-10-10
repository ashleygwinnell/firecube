#pragma once

#include "ComponentDescriptor.h"

class CharacterControllerDescriptor : public ComponentDescriptor
{
public:
	CharacterControllerDescriptor();
	virtual void CreateComponent(FireCube::Node *node, FireCube::Engine *engine);
	virtual ComponentDescriptor *Clone();
	virtual ~CharacterControllerDescriptor();

	void SetRadius(float radius);
	float GetRadius() const;
	void SetHeight(float height);
	float GetHeight() const;
	void SetContactOffset(float contactOffset);
	float GetContactOffset() const;
	void SetStepOffset(float stepOffset);
	float GetStepOffset() const;
private:
	float radius;
	float height;
	float contactOffset;
	float stepOffset;
};
