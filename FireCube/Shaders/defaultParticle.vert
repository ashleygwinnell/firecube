#if defined(POINT_LIGHT) || defined(SPOT_LIGHT)
	smooth out vec3 lightDir;
	smooth out vec3 eyeVec;
	uniform vec4 lightPosition;
#endif

#ifdef DIRECTIONAL_LIGHT	
	smooth out vec3 lightDir;
	smooth out vec3 eyeVec;
	uniform vec3 directionalLightDir;
#endif

layout (location = 0) in vec3 atrPosition;
layout (location = 2) in float atrLife;
layout (location = 3) in float atrLifeTime;

smooth out float ageFactor;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
uniform vec3 cameraPos;
uniform float particleSize;
uniform vec2 screenSize;

void  main()
{	
	#if defined(POINT_LIGHT) || defined(SPOT_LIGHT)
		vec3 vVertex = vec3(modelMatrix * vec4(atrPosition, 1.0));
		lightDir = vec3(lightPosition.xyz - vVertex) / lightPosition.w;
		eyeVec = cameraPos - vVertex;
	#endif
	#ifdef DIRECTIONAL_LIGHT
		lightDir = directionalLightDir;
		eyeVec = cameraPos - vec3(modelMatrix  * vec4(atrPosition, 1.0));		
	#endif
	
	ageFactor = (atrLifeTime - atrLife) / atrLifeTime;
	
	gl_Position = viewProjectionMatrix * modelMatrix * vec4(atrPosition, 1.0);
	gl_PointSize = screenSize.y * projectionMatrix[1][1] * particleSize /  gl_Position.w;
}