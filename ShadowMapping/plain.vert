#version 330
layout (location = 0) in vec3 atrPosition;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

void main()
{	
	gl_Position = projectionMatrix * modelViewMatrix * vec4(atrPosition, 1.0);
} 
