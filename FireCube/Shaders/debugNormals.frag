out vec4 outputColor;
smooth in vec2 texcoord;

#if !defined(NORMAL_MAPPING)
	smooth in vec3 normal;
#else
	smooth in mat3 tbn;
#endif

#ifdef NORMAL_MAPPING
	uniform sampler2D normalMap;
#endif	

uniform mat3 normalMatrix;

void main()
{		
	#ifdef NORMAL_MAPPING
		vec3 n = tbn * normalize(texture(normalMap, texcoord.xy).xyz * 2.0 - 1.0);
		n = normalize(normalMatrix * n);
	#else
		vec3 n = normalize(normal);
	#endif	
	
	outputColor = vec4(n, 1.0);
}