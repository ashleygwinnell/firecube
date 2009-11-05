varying vec3 normal;
uniform sampler2D texture[6];
//uniform int textured[6];
uniform int textured0;
uniform int textured1;
uniform int textured2;
uniform int textured3;
uniform int textured4;
uniform int textured5;

vec4 ApplyTextures()
{
	vec4 textureColor=vec4(1.0,1.0,1.0,1.0);
	if (textured0==1)
		textureColor*=texture2D(texture[0],gl_TexCoord[0].st);	
	if (textured1==1)
		textureColor*=texture2D(texture[1],gl_TexCoord[1].st);
	if (textured2==1)
		textureColor*=texture2D(texture[2],gl_TexCoord[2].st);
	if (textured3==1)
		textureColor*=texture2D(texture[3],gl_TexCoord[3].st);
	if (textured4==1)
		textureColor*=texture2D(texture[4],gl_TexCoord[4].st);
	if (textured5==1)
		textureColor*=texture2D(texture[5],gl_TexCoord[5].st);

	return textureColor;
	
}
void main()
{
	vec3 n=normalize(normal);
	vec3 lightDir=vec3(1.0,1.0,1.0) * gl_NormalMatrix;
	lightDir=normalize(lightDir);
	float d=max(dot(n,lightDir),0.0);
	gl_FragColor = gl_FrontMaterial.ambient*vec4(0.5,0.5,0.5,1.0);
	gl_FragColor +=gl_FrontMaterial.diffuse*d*ApplyTextures();
	gl_FragColor.a=gl_FrontMaterial.diffuse.a;	
}
