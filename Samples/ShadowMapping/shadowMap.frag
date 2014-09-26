#version 330

out vec4 outputColor;
uniform sampler2D shadowMap;
uniform sampler2D tex;
smooth in vec4 projShadow;
smooth in vec3 norm;
smooth in vec3 lightDir;

uniform vec4 materialAmbient;
uniform vec4 materialDiffuse;
uniform vec4 materialSpecular;
uniform float materialShininess;
smooth in vec2 texcoord;
float CalcShadowFactor(vec4 LightSpacePos, float ndotl)
{
	vec3 ProjCoords = LightSpacePos.xyz / LightSpacePos.w;
	vec2 UVCoords;
	UVCoords.x = ProjCoords.x;
        UVCoords.y = ProjCoords.y;
        if (clamp(UVCoords.x, 0, 1) != UVCoords.x)
           return 1.0;
        if (clamp(UVCoords.y, 0, 1) != UVCoords.y)
           return 1.0;
	float Depth = texture(shadowMap, UVCoords).x;
	if (LightSpacePos.w < 0.0)
		return 1.0;

        float bias = 0.00005;
        //float bias = 0.00005 * tan(acos(clamp(ndotl, 0, 1)));
        //bias = clamp(bias, 0, 0.00005);
	if (Depth < (ProjCoords.z + bias))
		return 0.2;
	else
		return 1.0;
} 
void main()
{
	vec4 ambient = materialAmbient * vec4(0.2,0.2,0.2,1);
	vec4 diffuse = materialDiffuse * vec4(1.0,1.0,1.0,1);

	float ndotl=max(dot(normalize(norm),normalize(lightDir)),0.0);	
	
	vec4 ts=texture2D(tex,texcoord);
	vec4 color=ambient;	
	float depth = CalcShadowFactor(projShadow, ndotl);
	color+=ndotl*diffuse * depth;
	color = color * ts;	
	outputColor = color;		
}
