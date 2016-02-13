out vec4 outputColor;
smooth in vec2 texcoord;
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
const float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
uniform vec2 viewportInvSize;
uniform vec2 blurhInvSize;
uniform vec2 blurvInvSize;
void main()
{
	vec3 diffColor = texture(diffuseMap, texcoord).rgb;
	#ifdef BRIGHT		
		const float bloomThreshold = 0.3;
		outputColor = vec4((diffColor - vec3(bloomThreshold)) / (1.0 - bloomThreshold), 1.0);		
	#elif defined(HBLUR)
		vec3 result = diffColor * weight[0]; // current fragment's contribution
		
        for(int i = 1; i < 5; ++i)
        {
            result += texture(diffuseMap, texcoord + vec2(viewportInvSize.x * i, 0.0)).rgb * weight[i];
            result += texture(diffuseMap, texcoord - vec2(viewportInvSize.x * i, 0.0)).rgb * weight[i];
        }    
		outputColor = vec4(result, 1.0);
	#elif defined(VBLUR)
		vec3 result = diffColor * weight[0]; // current fragment's contribution
		
        for(int i = 1; i < 5; ++i)
        {
            result += texture(diffuseMap, texcoord + vec2(0.0, viewportInvSize.y * i)).rgb * weight[i];
            result += texture(diffuseMap, texcoord - vec2(0.0, viewportInvSize.y * i)).rgb * weight[i];
        }   
		outputColor = vec4(result, 1.0);
	#elif defined(MIX)
		const float gamma = 2.2;		
		const float exposure = 0.5;
		vec3 bloomColor = texture(normalMap, texcoord).rgb;
		diffColor += bloomColor; // additive blending
		// tone mapping
		vec3 result = vec3(1.0) - exp(-diffColor * exposure);
		// also gamma correct while we're at it       
		result = pow(result, vec3(1.0 / gamma));
		outputColor = vec4(result, 1.0f);
	#endif	
}