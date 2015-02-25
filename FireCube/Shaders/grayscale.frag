out vec4 outputColor;
smooth in vec2 texcoord;
uniform sampler2D diffuseMap;

void main()
{
	vec3 diffColor = texture(diffuseMap, texcoord.xy).rgb;
	float v = dot(vec3(0.2126, 0.7152, 0.0722), diffColor);
	outputColor = vec4(v, v, v, 1.0);
}