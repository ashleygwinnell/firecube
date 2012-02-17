#version 330

out vec4 outputColor;
smooth in vec2 texcoord;

#if !defined(NORMAL_MAPPING)
	smooth in vec3 normal;
#endif

#if defined(DIRECTIONAL_LIGHTING) || defined(POINT_LIGHTING)
	uniform vec4 lightAmbient;
	uniform vec4 lightDiffuse;
	uniform vec4 lightSpecular;
	smooth in vec3 lightDir;
	smooth in vec3 eyeVec;
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
uniform vec4 materialAmbient;
uniform vec4 materialDiffuse;
uniform vec4 materialSpecular;
uniform float materialShininess;
void main()
{
	vec4 color = vec4(0.0, 0.0, 0.0, 1.0);	
	#if !defined(DIRECTIONAL_LIGHTING) && !defined(POINT_LIGHTING)
		#ifdef DIFFUSE_MAPPING
			color = materialDiffuse * texture(diffuseMap, texcoord.xy);
		#else
			color = materialDiffuse;
		#endif
	#else
		vec4 factor = lightAmbient * materialAmbient;
		#ifdef NORMAL_MAPPING
			vec3 n = normalize(texture(normalMap, texcoord.xy).xyz * 2.0 - 1.0);
		#else
			vec3 n = normalize(normal);
		#endif
		vec3 l = normalize(lightDir);
		float lambertTerm = dot(n, l);
		if (lambertTerm > 0.0)
		{
			factor += materialDiffuse * lightDiffuse * lambertTerm;
			vec3 E = normalize(eyeVec);
			vec3 R = reflect(-l, n);
			float specular = max(pow(max(dot(R, E), 0.0), materialShininess), 0.0);
			color += materialSpecular * lightSpecular * specular;
		}
		#ifdef DIFFUSE_MAPPING
			color += factor * texture(diffuseMap, texcoord.xy);
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
	
	outputColor = color;
}