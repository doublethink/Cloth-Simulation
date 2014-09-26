varying vec3 normal, eyeVec, halfVector, lightDir, halfVectorPoint, lightDirPoint;
varying float att;
varying vec4 diffuse, ambient;

void main()
{	
	normal = gl_NormalMatrix * gl_Normal;
	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	eyeVec = -vVertex;
	
	// Requirements for directional light
	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
    ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
    halfVector = normalize(gl_LightSource[0].halfVector.xyz);

	// Needed for spotlight
	lightDir = vec3(gl_LightSource[1].position.xyz - vVertex);
	att = 1.0 / gl_LightSource[1].constantAttenuation;
	
	// Needed for point light
	lightDirPoint = vec3(gl_LightSource[2].position.xyz - vVertex);
	halfVectorPoint = normalize(gl_LightSource[2].halfVector.xyz);
	
	// Texture coordinates
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

	gl_Position = ftransform();		
}