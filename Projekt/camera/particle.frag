uniform sampler2D texture;
//uniform float rotation;
uniform vec3 color;
uniform float time;
void main()
{
	float adjusted_rotation = time*0.125;//rotation*0.5;
	mat2 m2 = mat2(cos(adjusted_rotation), -sin(adjusted_rotation),sin(adjusted_rotation),	cos(adjusted_rotation));
	vec2 middle = {0.5,0.5};
	vec4 tex = texture2D(texture, (gl_PointCoord-middle)*(m2)+middle);
	if(tex.r > 0.9 && tex.g> 0.9 && tex.b> 0.9)
		discard;
	gl_FragColor = vec4(color,1-time/512);
}
