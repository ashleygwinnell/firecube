#if !defined(NORMAL_MAPPING)
	varying vec3 normal;
#endif

#if defined(DIRECTIONAL_LIGHTING) || defined(POINT_LIGHTING)
	uniform vec4 lightAmbient;
	uniform vec4 lightDiffuse;
	uniform vec4 lightSpecular;
	varying vec3 lightDir;
	varying vec3 eyeVec;
#endif
#ifdef DIFFUSE_MAPPING
	uniform sampler2D diffuseMap;
#endif
#ifdef NORMAL_MAPPING
	uniform sampler2D normalMap;
#endif	
#ifdef FOG
	uniform float fogDensity;
	uniform vec4 fogColor;
#endif		
void main()
{
	vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 factor = lightAmbient * gl_FrontMaterial.ambient;	
	#if !defined(DIRECTIONAL_LIGHTING) && !defined(POINT_LIGHTING)
		#ifdef DIFFUSE_MAPPING
			color = gl_FrontMaterial.diffuse * texture2D(diffuseMap, gl_TexCoord[0].xy);
		#else
			color = gl_FrontMaterial.diffuse;
		#endif
	#else
		#ifdef NORMAL_MAPPING
			vec3 n = normalize(texture2D(normalMap, gl_TexCoord[0].xy).xyz * 2.0 - 1.0);
		#else
			vec3 n = normalize(normal);
		#endif
		vec3 l = normalize(lightDir);
		float lambertTerm = dot(n, l);
		if (lambertTerm > 0.0)
		{
			factor += gl_FrontMaterial.diffuse * lightDiffuse * lambertTerm;
			vec3 E = normalize(eyeVec);
			vec3 R = reflect(-l, n);
			float specular = max(pow(max(dot(R, E), 0.0), gl_FrontMaterial.shininess), 0.0);
			color += gl_FrontMaterial.specular * lightSpecular * specular;
		}
		#ifdef DIFFUSE_MAPPING
			color += factor * texture2D(diffuseMap, gl_TexCoord[0].xy);
		#else
			color += factor;
		#endif
	#endif
	#ifdef FOG
		const float LOG2 = 1.442695;
		float z = gl_FragCoord.z / gl_FragCoord.w;
		float fogFactor = exp2(-fogDensity * fogDensity * z * z * LOG2);
		fogFactor = clamp(fogFactor, 0.0, 1.0);
		color = mix(fogColor, color, fogFactor );
	#endif
	
	gl_FragColor = color;
}