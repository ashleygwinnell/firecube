#version 330

out vec4 outputColor;

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
#ifdef FOG
	uniform vec3 fogParameters;
	uniform vec3 fogColor;
#endif
uniform vec3 ambientColor;
uniform vec4 materialAmbient;
uniform vec4 materialDiffuse;
uniform vec4 materialSpecular;
uniform float materialShininess;
void main()
{
	float alpha = 1.0;
	#ifdef DIFFUSE_MAPPING
		vec4 textureColor = texture(diffuseMap, gl_PointCoord);
		vec3 diffColor = materialDiffuse.rgb * textureColor.rgb;
		alpha = textureColor.a;
	#else			
		vec3 diffColor = materialDiffuse.rgb;			
	#endif
	
	#ifdef PER_PIXEL_LIGHTING
		vec3 color = vec3(0.0);
		
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
	
	vec2 d = gl_PointCoord - vec2(0.5, 0.5);
	if (d.x * d.x + d.y * d.y > 0.5 * 0.5)
		discard;
	outputColor = vec4(color, alpha);
}