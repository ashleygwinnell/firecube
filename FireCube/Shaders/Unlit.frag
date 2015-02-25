out vec4 outputColor;
smooth in vec2 texcoord;

#ifdef DIFFUSE_MAPPING
	uniform sampler2D diffuseMap;
#endif

uniform vec4 materialDiffuse;

void main()
{
	#ifdef DIFFUSE_MAPPING
		vec3 diffColor = materialDiffuse.rgb * texture(diffuseMap, texcoord.xy).rgb;
	#else			
		vec3 diffColor = materialDiffuse.rgb;			
	#endif
	outputColor = vec4(diffColor, 1.0);
}