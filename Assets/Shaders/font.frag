#version 330
out vec4 outputColor;
smooth in vec2 texcoord;
uniform vec4 textColor;
uniform sampler2D tex0;

void main()
{
	float alpha=texture(tex0, texcoord.st).r;
	outputColor = vec4(textColor.r,textColor.g,textColor.b,textColor.a*alpha);
}