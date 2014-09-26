varying vec3 normal, eyeVec, halfVector, lightDir, halfVectorPoint, lightDirPoint;
varying float att;
uniform sampler2D tex;
varying vec4 diffuse, ambient;

const float cos_outer_cone_angle = 0.85;	// Because spotCosCutoff does not appear to work

void main (void)
{
	// Start with Ambient light
	vec4 final_color = gl_LightModel.ambient * gl_FrontMaterial.ambient;

	// Add spotlight
	vec3 L = normalize(lightDir);
	vec3 D = normalize(gl_LightSource[1].spotDirection);

	float cos_cur_angle = dot(-L, D);
	vec3 N = normalize(normal);
	
	if( cos_cur_angle > cos_outer_cone_angle)
	{
		float falloff = (cos_cur_angle - cos_outer_cone_angle)/0.5;
		float lambertTerm = max( dot(N,L), 0.0);
		if(lambertTerm > 0.0)
		{
			final_color += gl_LightSource[1].diffuse * gl_FrontMaterial.diffuse * lambertTerm * falloff*att;	 
		
			vec3 E = normalize(eyeVec);
			vec3 R = reflect(-L, N);
			
			float specular = pow( max(dot(R, E), 0.0), gl_FrontMaterial.shininess );
			
			final_color += gl_LightSource[1].specular *	gl_FrontMaterial.specular *specular *falloff*att;	
		}
	}
	
	// Add directional lighting
	vec3 halfV;
	float NdotL, NdotHV;
	vec3 lightDirD = vec3(gl_LightSource[0].position);
	
	vec4 color = ambient;
	NdotL = max(dot(N, lightDirD),0.0);
	if (NdotL > 0.0){
		color += diffuse * NdotL;
		halfV = normalize(halfVector);
		NdotHV = max(dot(N, halfV), 0.0);
		color += gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(NdotHV, gl_FrontMaterial.shininess);
		final_color += color;
	}
	
	// Add point lighting	
	float dist = length(lightDirPoint);
	NdotL = max(dot(N,normalize(lightDirPoint)),0.0);
	if (NdotL > 0.0) {
		float attP = 1.0 / (gl_LightSource[2].constantAttenuation + gl_LightSource[2].linearAttenuation * dist + gl_LightSource[2].quadraticAttenuation * dist * dist);
		color = attP * (diffuse * NdotL + ambient);
		halfV = normalize(halfVectorPoint);
		NdotHV = max(dot(N,halfV),0.0);
		color += attP * gl_FrontMaterial.specular * gl_LightSource[2].specular * pow(NdotHV,gl_FrontMaterial.shininess);
		final_color += color;		
	}
	
	// Add textures
	vec4 texcolor = texture2D(tex,gl_TexCoord[0].st);
	final_color *= texcolor;
		
	
	gl_FragColor = final_color;
}