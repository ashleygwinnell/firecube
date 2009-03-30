varying vec3 normal;
uniform sampler2D texture[8];
uniform bool textured[8];

vec4 ApplyTextures()
{
	vec4 textureColor=vec4(1,1,1,1);
	for (int i=0;i<7;i++)
	{
		if (textured[i])
			textureColor*=texture2D(texture[i],gl_TexCoord[i].st);
	}
	return textureColor;
	
}
void main()
{
	vec4 color;
	vec3 n=normalize(normal);
	vec3 lightDir=vec3(1.0,1.0,1.0) * gl_NormalMatrix;
	lightDir=normalize(lightDir);
	float d=max(dot(n,lightDir),0.0);
	color = gl_FrontMaterial.ambient*vec4(0.5,0.5,0.5,1.0);
	color+=gl_FrontMaterial.diffuse*d*ApplyTextures();
	color.a=gl_FrontMaterial.diffuse.a;
	gl_FragColor=color;
}
