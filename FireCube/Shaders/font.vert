layout (location = 0) in vec3 atrPosition;
layout (location = 2) in vec2 atrTexCoord;
layout (location = 4) in vec4 atrColor;
smooth out vec2 texcoord;
smooth out vec4 textColor;
uniform mat4 projectionMatrix;

void main()
{
	texcoord = atrTexCoord;
	textColor = atrColor;
	gl_Position = projectionMatrix * vec4(atrPosition, 1.0);
}