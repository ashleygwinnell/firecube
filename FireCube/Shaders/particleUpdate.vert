layout (location = 0) in vec3 atrPosition;
layout (location = 1) in vec3 atrVelocity;
layout (location = 2) in float atrLife;
layout (location = 3) in float atrLifeTime;

out vec3 outPosition;
out vec3 outVelocity;
out float outLife;
out float outLifeTime;

uniform float timeStep;

void  main()
{
	if (atrLife < 0)
	{	
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