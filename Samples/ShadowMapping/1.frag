#version 330

out vec4 outputColor;
smooth in vec2 texcoord;

uniform sampler2D tex;
void main()
{
	outputColor = texture2D(tex,texcoord);	
}
