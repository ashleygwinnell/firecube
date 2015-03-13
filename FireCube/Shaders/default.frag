out vec4 outputColor;
smooth in vec2 texcoord;

#if !defined(NORMAL_MAPPING)
	smooth in vec3 normal;
#endif

#if defined(DIRECTIONAL_LIGHT) || defined(POINT_LIGHT) || defined(SPOT_LIGHT)	
	uniform vec4 lightColor;	
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
#ifdef SHADOW
	uniform sampler2D shadowMap;
	smooth in vec4 shadowCoord;
#endif
#ifdef FOG
	uniform vec3 fogParameters;
	uniform vec3 fogColor;
#endif
#ifndef PER_PIXEL_LIGHTING
	uniform vec3 ambientColor;
#endif
uniform vec4 materialDiffuse;
uniform vec4 materialSpecular;
uniform float materialShininess;
void main()
{
	float alpha = 1.0;
	#ifdef DIFFUSE_MAPPING
		vec4 textureColor = texture(diffuseMap, texcoord.xy);
		vec3 diffColor = materialDiffuse.rgb * textureColor.rgb;
		alpha = textureColor.a;
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
		float distanceAtten = 1.0;
		#ifdef POINT_LIGHT
			distanceAtten = 1.0 - pow(clamp(lightDirLength, 0.0, 1.0), 3);			
		#elif defined(SPOT_LIGHT)
			float spotCos = dot(l, normalize(spotLightDir.xyz));
			if (spotCos > spotLightDir.w)
			{				
				distanceAtten = 1.0 - pow(clamp(lightDirLength, 0.0, 1.0), 3);
				distanceAtten = distanceAtten * ((spotCos - spotLightDir.w) / (1.0 - spotLightDir.w));
			}
			else
			{
				distanceAtten = 0.0;
			}
		#endif
		#ifdef SHADOW				
			#ifdef SPOT_LIGHT
			if (textureProj(shadowMap, shadowCoord.xyw).x < min(shadowCoord.z / shadowCoord.w, 1.0))
			{
				lambertTerm *= 0.5;
			}
			#else
			if (texture(shadowMap, shadowCoord.xy).x < min(shadowCoord.z, 1.0))
			{
				lambertTerm *= 0.5;
			}
			#endif
		#endif
		
		color = distanceAtten * lightColor.rgb * lambertTerm * (diffColor +  materialSpecular.rgb * specular);
		
		#ifdef FOG
			/*const float LOG2 = 1.442695;
			float z = gl_FragCoord.z / gl_FragCoord.w;
			float fogFactor = exp2(-fogParameters.y * fogParameters.y * z * z * LOG2);
			fogFactor = clamp(fogFactor, 0.0, 1.0);
			color = mix(vec3(0), color, fogFactor );*/
			float z = gl_FragCoord.z / gl_FragCoord.w;
			float fogFactor = (z - fogParameters.x) * fogParameters.y;
			fogFactor = clamp(fogFactor, 0.0, 1.0);
			color = mix(color, vec3(0) , fogFactor);
		#endif
	#else
		vec3 color;
		color = diffColor * ambientColor;
		#ifdef FOG
			/*const float LOG2 = 1.442695;
			float z = gl_FragCoord.z / gl_FragCoord.w;
			float fogFactor = exp2(-fogParameters.y * fogParameters.y * z * z * LOG2);
			fogFactor = clamp(fogFactor, 0.0, 1.0);
			color = mix(fogColor, color, fogFactor );*/
			float z = gl_FragCoord.z / gl_FragCoord.w;
			float fogFactor = (z - fogParameters.x) * fogParameters.y;
			fogFactor = clamp(fogFactor, 0.0, 1.0);
			color = mix(color, fogColor, fogFactor );
		#endif
	#endif	
	
	outputColor = vec4(color, alpha);
}