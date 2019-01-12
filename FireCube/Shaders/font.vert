layout (location = 0) in vec3 atrPosition;
layout (location = 2) in vec2 atrTexCoord;
layout (location = 4) in vec4 atrColor;
smooth out vec2 texcoord;
smooth out vec4 inColor;
uniform mat4 projectionMatrix;
uniform vec2 positionOffset;

void main()
{
	texcoord = atrTexCoord;
	inColor = atrColor;
	gl_Position = projectionMatrix * vec4(vec3(positionOffset, 0.0) + atrPosition, 1.0);
}