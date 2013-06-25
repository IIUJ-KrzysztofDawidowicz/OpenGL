uniform float time;
uniform vec3 velocity; //Prêdkoœæ cz¹stki

const float constAtten  = 0.9;
const float linearAtten = 0.6;
const float quadAtten   = 0.001;

void main()
{
vec4 start = gl_Vertex;

//	if(time>0)	//Sprawdzanie tego w programie zamiast w szejderze poprawia sprawnoœæ, nie wiem dlaczego.
//	{			// Mo¿e ify s¹ w jakiœ sposób bardziej kosztowane w szejderach?


		float gravity = time*time/1024;
		
		vec4 move = {time*velocity.x/64,time*velocity.y/32-gravity,time*velocity.z/64,0}; 
		gl_Position = gl_ModelViewProjectionMatrix *(start + move);

		float dist = length(gl_Position);   //since the distance to (0,0,0) is just the length
		float attn = inversesqrt(constAtten + linearAtten*dist + quadAtten*dist*dist);
		
		gl_PointSize = (256-(time/2)) * attn;
//	}
}