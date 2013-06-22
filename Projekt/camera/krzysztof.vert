uniform float rotation;
varying vec3 vertex_light_position;
varying vec3 vertex_light_half_vector;
varying vec3 vertex_normal;
void main()
{	
	vec4 v = gl_Vertex;
	vertex_normal = normalize(gl_NormalMatrix * gl_Normal);
	float displacement = sin(v.x*v.y*v.z*rotation)-1;
	vec3 displacement_direction = {vertex_normal.y,vertex_normal.z,vertex_normal.x};
	vertex_light_position = normalize(gl_LightSource[0].position.xyz);
	vertex_light_half_vector = normalize(gl_LightSource[0].halfVector.xyz);
	gl_Position = ftransform();		
	//gl_Position.xyz = (v*gl_ModelViewMatrix).xyz*(1-displacement*0.25);
	//gl_Position = v*gl_ModelViewMatrix;
	gl_Position.xyz += displacement_direction*(-displacement*0.25);
	gl_TexCoord[0] = gl_MultiTexCoord0;
	//gl_TexCoord[0].s +=sin(rotation); 
}