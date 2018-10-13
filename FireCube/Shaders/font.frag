out vec4 outputColor;
smooth in vec2 texcoord;
smooth in vec4 inColor;
uniform sampler2D tex0;

void main()
{
	float alpha=texture(tex0, texcoord.st).r;
	outputColor = vec4(inColor.r,inColor.g,inColor.b,inColor.a*alpha);
}