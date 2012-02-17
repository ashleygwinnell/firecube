#version 330

#ifdef POINT_LIGHTING
	smooth out vec3 lightDir;
	smooth out vec3 eyeVec;
	uniform vec3 lightPosition;
#endif

#ifdef DIRECTIONAL_LIGHTING	
	smooth out vec3 lightDir;
	smooth out vec3 eyeVec;
	uniform vec3 directionalLightDir;
#endif

layout (location = 0) in vec3 atrPosition;
layout (location = 1) in vec3 atrNormal;
layout (location = 2) in vec3 atrTangent;
layout (location = 3) in vec3 atrBitangent;
layout (location = 4) in vec2 atrTexCoord;

#if !defined(NORMAL_MAPPING)
	smooth out vec3 normal;
#endif
smooth out vec2 texcoord;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
void  main()
{
	texcoord = atrTexCoord;
	#ifdef POINT_LIGHTING	
		vec3 vVertex = vec3(modelViewMatrix * vec4(atrPosition, 1.0));
		lightDir= vec3(lightPosition - vVertex);
		eyeVec = -vVertex;
	#endif
	#ifdef DIRECTIONAL_LIGHTING
		lightDir = directionalLightDir;
		eyeVec = -vec3(modelViewMatrix  * vec4(atrPosition, 1.0));		
	#endif
	#ifdef NORMAL_MAPPING
		mat3 tbn = mat3(normalize(atrTangent), normalize(atrBitangent), normalize(atrNormal));
		eyeVec = eyeVec * normalMatrix * tbn;
		lightDir = lightDir * normalMatrix * tbn;
	#endif
	#if !defined(NORMAL_MAPPING)
		normal = normalMatrix * atrNormal;
	#endif
	gl_Position = projectionMatrix * modelViewMatrix * vec4(atrPosition, 1.0);
}