#version 330

layout (location = 0) in vec3 atrPosition;
layout (location = 1) in vec3 atrNormal;
layout (location = 2) in vec2 atrTexCoord;
layout (location = 3) in vec3 atrTangent;

#if !defined(NORMAL_MAPPING)
	smooth out vec3 normal;
#else
	smooth out mat3 tbn;
#endif
smooth out vec2 texcoord;
uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;
uniform mat3 normalMatrix;
uniform vec3 cameraPos;
void  main()
{
	texcoord = atrTexCoord;	
	#ifdef NORMAL_MAPPING
		vec3 bitangent = cross(atrTangent, atrNormal);
		tbn = mat3(normalize(normalMatrix * atrTangent), normalize(normalMatrix * bitangent), normalize(normalMatrix * atrNormal));		
	#else
		normal = normalMatrix * atrNormal;		
	#endif
	gl_Position = viewProjectionMatrix * modelMatrix * vec4(atrPosition, 1.0);
}