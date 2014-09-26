#version 330
layout (location = 0) in vec2 atrPosition;
layout (location = 4) in vec2 atrTexCoord;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

smooth out vec2 texcoord;

void main()
{	
	texcoord = atrTexCoord;
	gl_Position = projectionMatrix * modelViewMatrix * vec4(atrPosition, 0.0, 1.0);
} 
