varying vec4 projShadow;
varying vec3 norm;
void main()
{
	norm = vec3(gl_Normal * gl_NormalMatrix);
	projShadow = gl_TextureMatrix[1] * gl_Vertex;
	gl_TexCoord[0]=gl_MultiTexCoord0;
	gl_Position = ftransform();
} 
