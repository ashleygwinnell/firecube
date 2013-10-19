#version 330

out vec4 outputColor;
smooth in vec2 texcoord;

#if !defined(NORMAL_MAPPING)
	smooth in vec3 normal;
#endif

#if defined(DIRECTIONAL_LIGHT) || defined(POINT_LIGHT) || defined(SPOT_LIGHT)
	uniform vec4 lightAmbient;
	uniform vec4 lightDiffuse;
	uniform vec4 lightSpecular;
	smooth in vec3 lightDir;
	smooth in vec3 eyeVec;
#endif

#ifdef SPOT_LIGHT
	uniform vec4 spotLightDir;
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
uniform vec3 ambientColor;
uniform vec4 materialAmbient;
uniform vec4 materialDiffuse;
uniform vec4 materialSpecular;
uniform float materialShininess;
void main()
{
	#ifdef DIFFUSE_MAPPING
		vec3 diffColor = materialDiffuse.rgb * texture(diffuseMap, texcoord.xy).rgb;
	#else			
		vec3 diffColor = materialDiffuse.rgb;			
	#endif
	
	#ifdef PER_PIXEL_LIGHTING
		vec3 color = vec3(0.0);
		#ifdef NORMAL_MAPPING
			vec3 n = normalize(texture(normalMap, texcoord.xy).xyz * 2.0 - 1.0);
		#else
			vec3 n = normalize(normal);
		#endif
		float lightDirLength = length(lightDir);
		vec3 l = lightDir / lightDirLength;
		float lambertTerm = max(dot(n, l), 0.0);
		vec3 E = normalize(eyeVec);
		vec3 R = reflect(-l, n);
		float specular = max(pow(max(dot(R, E), 0.0), materialShininess), 0.0);
		#ifdef POINT_LIGHT
			float distanceAtten = 1.0 - pow(clamp(lightDirLength, 0.0, 1.0), 3);
			color = distanceAtten * lightDiffuse.rgb * lambertTerm * (diffColor +  materialSpecular.rgb * lightSpecular.rgb * specular);
		#elif defined(SPOT_LIGHT)
			float spotCos = dot(l, normalize(spotLightDir.xyz));
			if (spotCos > spotLightDir.w)
			{				
				float distanceAtten = 1.0 - pow(clamp(lightDirLength, 0.0, 1.0), 3);
				distanceAtten = distanceAtten * (1.0 - (1.0 - spotCos) / (1.0 - spotLightDir.w));
				color = distanceAtten * lightDiffuse.rgb * lambertTerm * (diffColor +  materialSpecular.rgb * lightSpecular.rgb * specular);
			}
		#else
			color = lightDiffuse.rgb * lambertTerm * (diffColor +  materialSpecular.rgb * lightSpecular.rgb * specular);
		#endif
	#else
		vec3 color;
		color = diffColor * ambientColor;
	#endif
	#ifdef FOG
		const float LOG2 = 1.442695;
		float z = gl_FragCoord.z / gl_FragCoord.w;
		float fogFactor = exp2(-fogDensity * fogDensity * z * z * LOG2);
		fogFactor = clamp(fogFactor, 0.0, 1.0);
		color = mix(fogColor, color, fogFactor );
	#endif
	
	outputColor = vec4(color, 1.0);
}