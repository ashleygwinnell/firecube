uniform sampler2D tex0;
void main()
{	
	gl_FragColor = texture2D(tex0,gl_TexCoord[0].st);
	//gl_FragColor = vec4(1,1,1,1);
}
