#pragma once

#include "ComponentDescriptor.h"

class CameraDescriptor : public ComponentDescriptor
{
public:
	CameraDescriptor();
	virtual void CreateComponent(FireCube::Node *node, FireCube::Engine *engine);
	virtual ComponentDescriptor *Clone();
	virtual ~CameraDescriptor();

	void SetOrthographic(bool orthographic);
	bool GetOrthographic() const;
	void SetFOV(float fov);
	float GetFOV() const;
	void SetNearPlane(float nearPlane);
	float GetNearPlane() const;
	void SetFarPlane(float farPlane);	
	float GetFarPlane() const;
	void SetLeftPlane(float leftPlane);
	float GetLeftPlane() const;
	void SetRightPlane(float rightPlane);	
	float GetRightPlane() const;
	void SetTopPlane(float topPlane);	
	float GetTopPlane() const;
	void SetBottomPlane(float bottomPlane);
	float GetBottomPlane() const;

private:
	float fov;
	float nearPlane;
	float farPlane;	

	float leftPlane;
	float rightPlane;
	float topPlane;
	float bottomPlane;

	bool orthographic;
};