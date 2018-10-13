out vec4 outputColor;
smooth in vec2 texcoord;
smooth in vec4 inColor;
uniform sampler2D tex0;

void main()
{
	vec4 color = texture(tex0, texcoord.st);
	#ifdef ALPHA
		outputColor = color * inColor;
	#else
		outputColor = vec4(color.rgb * inColor.rgb, 1.0);
	#endif
}