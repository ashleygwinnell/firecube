out vec4 outputColor;
smooth in vec3 color;
void main()
{
	outputColor = vec4(color, 1.0);
}