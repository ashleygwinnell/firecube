#version 330

#ifdef POINT_LIGHT
	smooth out vec3 lightDir;
	smooth out vec3 eyeVec;
	uniform vec3 lightPosition;
#endif

#ifdef DIRECTIONAL_LIGHT	
	smooth out vec3 lightDir;
	smooth out vec3 eyeVec;
	uniform vec3 directionalLightDir;
#endif

layout (location = 0) in vec3 atrPosition;
layout (location = 1) in vec3 atrNormal;
layout (location = 2) in vec2 atrTexCoord;
layout (location = 3) in vec3 atrTangent;
layout (location = 4) in vec3 atrBitangent;

#if !defined(NORMAL_MAPPING)
	smooth out vec3 normal;
#endif
smooth out vec2 texcoord;
uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;
uniform mat3 normalMatrix;
void  main()
{
	texcoord = atrTexCoord;
	#ifdef POINT_LIGHT	
		vec3 vVertex = vec3(modelMatrix * vec4(atrPosition, 1.0));
		lightDir = vec3(lightPosition - vVertex);
		eyeVec = -vVertex;
	#endif
	#ifdef DIRECTIONAL_LIGHT
		lightDir = directionalLightDir;
		eyeVec = -vec3(modelMatrix  * vec4(atrPosition, 1.0));		
	#endif
	#ifdef NORMAL_MAPPING
		mat3 tbn = mat3(normalize(atrTangent), normalize(atrBitangent), normalize(atrNormal));
		eyeVec = eyeVec * normalMatrix * tbn;
		lightDir = lightDir * normalMatrix * tbn;
	#endif
	#if !defined(NORMAL_MAPPING)
		normal = normalMatrix * atrNormal;
	#endif
	gl_Position = viewProjectionMatrix * modelMatrix * vec4(atrPosition, 1.0);
}