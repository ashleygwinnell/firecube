varying vec4 col;
uniform vec3 lightDir;
void main()
{	
	vec3 normal = normalize(gl_NormalMatrix * gl_Normal);			
	vec4 diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	vec4 ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;		
	
	float NdotL=max(-dot(normal,lightDir),0.0);
	col =ambient + diffuse * NdotL;
	gl_TexCoord[0]=gl_MultiTexCoord0;
	gl_Position = ftransform();
} 
