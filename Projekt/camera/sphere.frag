uniform sampler2D texture1;
uniform sampler2D texture2;
varying vec3 vertex_light_position;
varying vec3 vertex_light_half_vector;
varying vec3 vertex_normal;
uniform float alpha;

//Zamieñ miejscami kolory w teksturze
vec4 naprawTeksture(vec4 tex);

float calculateDiffuseValue(vec3 norm);
vec4  calculateDiffuseColor(vec4 tex1, vec4 tex2);
vec4  calculateDiffuseColor(vec4 tex1, vec4 tex2, float diffuse_value);
vec4  calculateSpecularColor(vec3 norm);
vec4  calculateAmbientColor();
bool  isFragmentBlue(vec4 tex);
vec4  combineColor(vec4 color1, vec4 color2);
vec4  combineColor(vec4 color1, vec4 color2, vec4 color3);

void main (void)
{

	vec3 norm = normalize(vertex_normal);
	vec4 tex1 = texture2D(texture1, gl_TexCoord[0].st);
	vec4 tex2 = texture2D(texture2, gl_TexCoord[0].st);

	tex1 = naprawTeksture(tex1); //Z jakiegoœ powodu w tej teksturze s¹ prestawione kolory


	float diffuse_value = calculateDiffuseValue(norm);
	vec4 diffuse_color = tex1 * diffuse_value;
	vec4 specular_color = calculateSpecularColor(norm);
	vec4 ambient_color = calculateAmbientColor();

	bool showSpecular = isFragmentBlue(diffuse_color);

	if(showSpecular)
	{
		gl_FragColor = combineColor(diffuse_color, specular_color, ambient_color);
	}
	else
	{
		gl_FragColor = diffuse_color + ambient_color;
	}
	
}

//Zamieñ miejscami kolory w teksturze
vec4 naprawTeksture(vec4 tex)
{
	float tmp = tex[0];
	tex[0]=tex[1];
	tex[1]=tex[2];
	tex[2]=tmp;
	return tex;
}

float calculateDiffuseValue(vec3 norm)
{
	return max(dot(norm, vertex_light_position), 0.0);
}

vec4 calculateDiffuseColor(vec4 tex1, vec4 tex2)
{
	return (tex1 * alpha) + (tex2 * (1 - alpha));
}

vec4 calculateDiffuseColor(vec4 tex1, vec4 tex2, float diffuse_value)
{
	return calculateDiffuseColor(tex1, tex2)*diffuse_value;
}

vec4 calculateSpecularColor(vec3 norm)
{
	return gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(max(dot(norm, vertex_light_half_vector), 0.0) , gl_FrontMaterial.shininess);
}

vec4 calculateAmbientColor()
{
	return 0.125 * gl_FrontMaterial.ambient * gl_LightSource[0].ambient + gl_LightModel.ambient * gl_FrontMaterial.ambient;
}

bool isFragmentBlue(vec4 tex)
{
	return !(tex[0]>0.25 & tex[1]<0.8 & tex[2]<0.8);
}

vec4  combineColor(vec4 color1, vec4 color2)
{
	return color1 + color2;
}
vec4  combineColor(vec4 color1, vec4 color2, vec4 color3)
{
	return color1 + color2 + color3;
}