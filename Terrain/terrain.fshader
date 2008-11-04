uniform sampler2D tex;
uniform float fogDensity;
uniform vec4 fogColor;
varying vec4 col;
void main()
{
	vec4 color=col;
	color*=texture2D(tex,gl_TexCoord[0].st);
	const float LOG2 = 1.442695;
	float z = gl_FragCoord.z / gl_FragCoord.w;
	float fogFactor = exp2( -fogDensity* fogDensity* z * z * LOG2 );
	fogFactor = clamp(fogFactor, 0.0, 1.0);
	color = mix(fogColor, color, fogFactor );
	gl_FragColor=color;

}
