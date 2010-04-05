varying vec3 normal;
void main()
{
	gl_FragColor=gl_FrontMaterial.ambient+gl_FrontMaterial.diffuse;
}
