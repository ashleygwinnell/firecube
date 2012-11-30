#if !defined(NORMAL_MAPPING)
	varying vec3 normal;
#endif

#ifdef NORMAL_MAPPING
	uniform sampler2D normalMap;
	varying mat3 tbn;
#endif	
void main()
{
	vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
	#ifdef NORMAL_MAPPING
		vec3 n = tbn * normalize(texture2D(normalMap, gl_TexCoord[0].xy).xyz * 2.0 - 1.0);
		n = gl_NormalMatrix * n ;

	#else
		vec3 n = normalize(normal);
	#endif
	n = (normalize(n) + 1.0) / 2.0;
	color = vec4(n, 1.0);

	gl_FragColor = color;
}