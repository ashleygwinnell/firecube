#version 330

layout (location = 0) in vec3 atrPosition;
layout (location = 1) in vec3 atrNormal;
layout (location = 2) in vec2 atrTexCoord;
layout (location = 3) in vec3 atrTangent;

smooth out vec2 texcoord;
uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;
uniform mat3 normalMatrix;
uniform vec3 cameraPos;
void  main()
{
	texcoord = atrTexCoord;
	gl_Position = viewProjectionMatrix * modelMatrix * vec4(atrPosition, 1.0);
}