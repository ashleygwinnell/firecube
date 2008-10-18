uniform sampler2D tex,terrainMap,detailMap;
uniform float fogDensity;
uniform vec4 fogColor;
void main()
{
	vec4 color;
	color=texture2D(terrainMap,gl_TexCoord[0].st);
	color=color*texture2D(detailMap,vec2(gl_TexCoord[0].s*20.0,gl_TexCoord[0].t*20.0));
	const float LOG2 = 1.442695;
	float z = gl_FragCoord.z / gl_FragCoord.w;
	float fogFactor = exp2( -fogDensity* fogDensity* z * z * LOG2 );
	fogFactor = clamp(fogFactor, 0.0, 1.0);
	color = mix(fogColor, color, fogFactor );
	gl_FragColor=color;

}
