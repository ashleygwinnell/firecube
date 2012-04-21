#version 330

layout (location = 0) in vec3 atrPosition;
layout (location = 1) in vec3 atrNormal;
layout (location = 4) in vec2 atrTexCoord;

smooth out vec4 projShadow;
smooth out vec3 norm;
smooth out vec3 lightDir;
uniform mat4 lightMatrix;
uniform vec3 lightPos;

smooth out vec2 texcoord;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
void main()
{	
	lightDir = normalMatrix * vec3(lightPos - atrPosition);
	norm = normalMatrix * atrNormal;
	projShadow = lightMatrix * vec4(atrPosition, 1.0);
	texcoord = atrTexCoord;
	gl_Position = projectionMatrix * modelViewMatrix * vec4(atrPosition, 1.0);
} 
