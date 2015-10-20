out vec4 outputColor;

#if defined(DIRECTIONAL_LIGHT) || defined(POINT_LIGHT) || defined(SPOT_LIGHT)
	uniform vec3 lightColor;	
	smooth in vec3 lightDir;
	smooth in vec3 eyeVec;
#endif

#ifdef SPOT_LIGHT
	uniform vec4 spotLightDir;
#endif

#ifdef DIFFUSE_MAPPING
	uniform sampler2D diffuseMap;
#endif
#ifdef FOG
	uniform vec3 fogParameters;
	uniform vec3 fogColor;
#endif
uniform vec3 startColor;
uniform vec3 endColor;
uniform float startOpacity;
uniform float endOpacity;
smooth in float ageFactor;

void main()
{
	float alpha = mix(startOpacity, endOpacity, ageFactor);
	vec3 color = mix(startColor, endColor, ageFactor);
	#ifdef DIFFUSE_MAPPING
		vec4 textureColor = texture(diffuseMap, gl_PointCoord);
		color *= textureColor.rgb;
		alpha *= textureColor.a;	
	#endif
			
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
	
	vec2 d = gl_PointCoord - vec2(0.5, 0.5);
	if (d.x * d.x + d.y * d.y > 0.5 * 0.5)
		discard;
	outputColor = vec4(color, alpha);
}