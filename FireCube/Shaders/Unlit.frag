out vec4 outputColor;
smooth in vec2 texcoord;

#ifdef DIFFUSE_MAPPING
	uniform sampler2D diffuseMap;
#endif

uniform vec3 materialDiffuse;
uniform float materialOpacity;

void main()
{
	float alpha = materialOpacity;
	#ifdef DIFFUSE_MAPPING
		vec4 textureColor = texture(diffuseMap, texcoord.xy);
		vec3 diffColor = materialDiffuse * textureColor.rgb;
		alpha *= textureColor.a;
	#else			
		vec3 diffColor = materialDiffuse;			
	#endif
	outputColor = vec4(diffColor, alpha);
}