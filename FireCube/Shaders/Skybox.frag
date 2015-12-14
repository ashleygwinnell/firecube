out vec4 outputColor;
smooth in vec3 texcoord;

uniform samplerCube diffuseMap;

void main()
{
	outputColor = texture(diffuseMap, texcoord);
}