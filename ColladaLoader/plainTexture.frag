varying vec3 normal;
uniform sampler2D diffuseMap;
void main()
{

	vec4 color=vec4(1,1,1,1);
	color*=texture2D(diffuseMap,gl_TexCoord[0].xy);
	gl_FragColor=color;
}
