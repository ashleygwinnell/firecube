#include "CharacterControllerDescriptor.h"

using namespace FireCube;

CharacterControllerDescriptor::CharacterControllerDescriptor() : ComponentDescriptor(ComponentType::CHARACTER_CONTROLLER), contactOffset(0.1f), radius(0.5f), height(1.0f), stepOffset(0.3f)
{

}

void CharacterControllerDescriptor::CreateComponent(FireCube::Node *node, FireCube::Engine *engine)
{
	auto characterController = node->CreateComponent<CharacterController>();
	characterController->SetRadius(radius);
	characterController->SetHeight(height);
	characterController->SetContactOffset(contactOffset);
	characterController->SetStepOffset(stepOffset);

	component = characterController;
}

ComponentDescriptor *CharacterControllerDescriptor::Clone()
{
	auto clone = new CharacterControllerDescriptor();
	clone->radius = radius;
	clone->height = height;
	clone->contactOffset = contactOffset;
	clone->stepOffset = stepOffset;

	return clone;
}

CharacterControllerDescriptor::~CharacterControllerDescriptor()
{

}

void CharacterControllerDescriptor::SetRadius(float radius)
{
	this->radius = radius;
	if (component)
	{
		((CharacterController *)component)->SetRadius(radius);
	}
}

float CharacterControllerDescriptor::GetRadius() const
{
	return radius;
}

void CharacterControllerDescriptor::SetHeight(float height)
{
	this->height = height;
	if (component)
	{
		((CharacterController *)component)->SetHeight(height);
	}
}

float CharacterControllerDescriptor::GetHeight() const
{
	return height;
}

void CharacterControllerDescriptor::SetContactOffset(float contactOffset)
{
	this->contactOffset = contactOffset;
	if (component)
	{
		((CharacterController *)component)->SetContactOffset(contactOffset);
	}
}

float CharacterControllerDescriptor::GetContactOffset() const
{
	return contactOffset;
}

void CharacterControllerDescriptor::SetStepOffset(float stepOffset)
{
	this->stepOffset = stepOffset;
	if (component)
	{
		((CharacterController *)component)->SetStepOffset(stepOffset);
	}
}

float CharacterControllerDescriptor::GetStepOffset() const
{
	return stepOffset;
}