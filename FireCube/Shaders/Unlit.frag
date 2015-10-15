out vec4 outputColor;
smooth in vec2 texcoord;

#ifdef DIFFUSE_MAPPING
	uniform sampler2D diffuseMap;
#endif

uniform vec3 materialDiffuse;
uniform float materialOpacity;

void main()
{
	#ifdef DIFFUSE_MAPPING
		vec3 diffColor = materialDiffuse * texture(diffuseMap, texcoord.xy).rgb;
	#else			
		vec3 diffColor = materialDiffuse;			
	#endif
	outputColor = vec4(diffColor, materialOpacity);
}