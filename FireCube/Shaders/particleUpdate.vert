#version 330

layout (location = 0) in vec3 atrPosition;
layout (location = 1) in vec3 atrVelocity;
layout (location = 2) in float atrAge;

out vec3 outPosition;
out vec3 outVelocity;
out float outAge;

void  main()
{
	outPosition = atrPosition + atrVelocity;
	outVelocity = atrVelocity;
	outAge = atrAge;	
}