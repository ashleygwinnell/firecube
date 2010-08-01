uniform sampler2DShadow shadowMap;
uniform sampler2D tex;
varying vec4 projShadow;
varying vec3 norm;
uniform vec3 lightPos;
void main()
{
	vec3 diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	vec3 ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;	

	vec3 lightDir=normalize(vec3(vec4(lightPos,1.0) * gl_ModelViewMatrix));
	float ndotl=max(dot(norm,lightDir),0.0);	
	
	vec3 ts=vec3(texture2D(tex,gl_TexCoord[0].st));
	vec3 color=ambient*ts;
	color+=ts*ndotl*diffuse*shadow2DProj(shadowMap, projShadow);	
	gl_FragColor = vec4(color, 1.0);	
}
