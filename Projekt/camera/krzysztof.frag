uniform sampler2D texture1;
uniform sampler2D texture2;
varying vec3 vertex_light_position;
varying vec3 vertex_light_half_vector;
varying vec3 vertex_normal;
uniform float alpha;

void main (void)
{

	vec3 norm = normalize(vertex_normal);
	vec4 tex1 = texture2D( texture1, gl_TexCoord[0].st);
	vec4 tex2 = texture2D( texture2, gl_TexCoord[0].st);
	float tmp = tex1[0]; //napraw teksturê
	tex1[0]=tex1[1];
	tex1[1]=tex1[2];
	tex1[2]=tmp;
	float diffuse_value = max(dot(norm, vertex_light_position), 0.0);
	vec4 diffuse_color = tex1*alpha+tex2*(1-alpha)/* diffuse_value*/;
	vec4 specular_color = gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(max(dot(norm, vertex_light_half_vector), 0.0) , gl_FrontMaterial.shininess);
	vec4 ambient_color = gl_FrontMaterial.ambient * gl_LightSource[0].ambient + gl_LightModel.ambient * gl_FrontMaterial.ambient;
	//if(tex1[0]>0.25 & tex1[1]<0.8 & tex1[2]<0.8)
	//{
		gl_FragColor = diffuse_color /* + specular_color + ambient_color*/;
	//}
	//else
	//{
	//	gl_FragColor = diffuse_color + specular_color;
	//}
	
}
