#version 330

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
layout (location = 1) in vec3 atrNormal;
layout (location = 2) in vec2 atrTexCoord;
layout (location = 3) in vec3 atrTangent;

#if !defined(NORMAL_MAPPING)
	smooth out vec3 normal;
#endif
smooth out vec2 texcoord;
uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;
uniform mat3 normalMatrix;
uniform vec3 cameraPos;
void  main()
{
	texcoord = atrTexCoord;
	#if defined(POINT_LIGHT) || defined(SPOT_LIGHT)
		vec3 vVertex = vec3(modelMatrix * vec4(atrPosition, 1.0));
		lightDir = vec3(lightPosition.xyz - vVertex) / lightPosition.w;
		eyeVec = cameraPos - vVertex;
	#endif
	#ifdef DIRECTIONAL_LIGHT
		lightDir = directionalLightDir;
		eyeVec = cameraPos - vec3(modelMatrix  * vec4(atrPosition, 1.0));		
	#endif
	#ifdef NORMAL_MAPPING
		vec3 bitangent = cross(atrTangent, atrNormal);
		mat3 tbn = mat3(normalize(normalMatrix * atrTangent), normalize(normalMatrix * bitangent), normalize(normalMatrix * atrNormal));
		eyeVec = eyeVec * tbn;
		lightDir = lightDir * tbn;
	#endif
	#if !defined(NORMAL_MAPPING)
		normal = normalMatrix * atrNormal;
	#endif
	gl_Position = viewProjectionMatrix * modelMatrix * vec4(atrPosition, 1.0);
}