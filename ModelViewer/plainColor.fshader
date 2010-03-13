varying vec3 normal;
uniform sampler2D diffuseMap;
void main()
{
	vec3 n=normalize(normal);
	vec3 lightDir=vec3(1.0,1.0,1.0) * gl_NormalMatrix;
	lightDir=normalize(lightDir);
	float d=max(dot(n,lightDir),0.0);
	gl_FragColor = gl_FrontMaterial.ambient*vec4(0.5,0.5,0.5,1.0);
	gl_FragColor +=gl_FrontMaterial.diffuse*d*texture2D(diffuseMap,gl_TexCoord[0].xy);
	gl_FragColor.a=gl_FrontMaterial.diffuse.a;	
}
