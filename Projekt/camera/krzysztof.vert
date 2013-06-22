uniform float rotation;
varying vec3 vertex_light_position;
varying vec3 vertex_light_half_vector;
varying vec3 vertex_normal;

float calculateDisplacement(vec4 v);
vec3  calculateDisplacementDirection();
vec3  normalizeLightPosition();
vec3  normalizeHalfVector();
vec4  calculatePosition(float displacement, vec3 displacement_direction);

void main()
{	
	vertex_normal = normalize(gl_NormalMatrix * gl_Normal);
	vertex_light_position = normalizeLightPosition();
	vertex_light_half_vector = normalizeHalfVector();

	float displacement = calculateDisplacement(gl_Vertex);
	vec3 displacement_direction = calculateDisplacementDirection();
	gl_Position = calculatePosition(displacement, displacement_direction);

	gl_TexCoord[0] = gl_MultiTexCoord0;
	//gl_TexCoord[0].s +=sin(rotation); 
}

float calculateDisplacement(vec4 v)
{
	return sin(v.x * v.y * v.z * rotation);
}

vec3  calculateDisplacementDirection()
{
	vec3 twistedNormal = vertex_normal.yzx;
	vec3 zeroVector = {0,0,0};
	if(distance(vertex_normal, twistedNormal)>0.0625)
		return cross(vertex_normal, twistedNormal);
	return zeroVector;
}


vec3  normalizeLightPosition()
{
	return normalize(gl_LightSource[0].position.xyz);
}


vec3  normalizeHalfVector()
{
	return normalize(gl_LightSource[0].halfVector.xyz);
}


vec4  calculatePosition(float displacement, vec3 displacement_direction)
{
	vec4 position = ftransform();
	position.xyz += displacement_direction*(-displacement*0.25); //Displace
	return position;

	//Poprzednie próby kalkulacji
	//gl_Position.xyz = (v*gl_ModelViewMatrix).xyz*(1-displacement*0.25);
	//gl_Position = v*gl_ModelViewMatrix;
}