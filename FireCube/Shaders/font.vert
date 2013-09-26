#version 330
layout (location = 0) in vec3 atrPosition;
layout (location = 2) in vec2 atrTexCoord;
smooth out vec2 texcoord;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	texcoord = atrTexCoord;
	gl_Position = projectionMatrix * modelViewMatrix * vec4(atrPosition, 1.0);
}