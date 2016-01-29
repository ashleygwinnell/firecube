layout (location = 0) in vec3 atrPosition;
layout (location = 1) in vec3 atrVelocity;
layout (location = 2) in float atrLife;
layout (location = 3) in float atrLifeTime;

out vec3 outPosition;
out vec3 outVelocity;
out float outLife;
out float outLifeTime;

uniform float timeStep;

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
	if (atrLife < 0)
	{	
		//uint seed = uint(atrAge * 1000.0) + uint(gl_VertexID);
		//float x = randhash(seed++, 1.0) * 2.0 - 1.0;		
		outPosition = vec3(1e6);		
		outLife = atrLife;
	}
	else
	{		
		outPosition = atrPosition + atrVelocity * timeStep;
		outLife = atrLife - timeStep;
	}		
	
	outVelocity = atrVelocity;
	outLifeTime = atrLifeTime;
}