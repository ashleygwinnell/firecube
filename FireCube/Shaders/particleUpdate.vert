#version 330

layout (location = 0) in vec3 atrPosition;
layout (location = 1) in vec3 atrVelocity;
layout (location = 2) in float atrAge;

out vec3 outPosition;
out vec3 outVelocity;
out float outAge;

uniform float timeStep;
uniform float lifeTime;

const float TwoPi = 6.28318530718;
const float InverseMaxInt = 1.0 / 4294967295.0;
 
float randhash(uint seed, float b)
{
    uint i=(seed^12345391u)*2654435769u;
    i^=(i<<6u)^(i>>26u);
    i*=2654435769u;
    i+=(i<<5u)^(i>>12u);
    return float(b * i) * InverseMaxInt;
}

void  main()
{
	if (atrAge > lifeTime)	
	{	
		//uint seed = uint(atrAge * 1000.0) + uint(gl_VertexID);
		//float x = randhash(seed++, 1.0) * 2.0 - 1.0;		
		outPosition = vec3(0.0);
		outAge = atrAge - lifeTime;
	}
	else
	{		
		outPosition = atrPosition + atrVelocity * timeStep;
		outAge = atrAge + timeStep;
	}		
	
	outVelocity = atrVelocity;
}