#version 150 compatibility

// eye cooerdinate
out vec3 vertexEyeSpace;
// normal vector
out vec3 normal;

void main(){
	vertexEyeSpace = vec3(gl_ModelViewMatrix * gl_Vertex);
	normal = normalize(gl_NormalMatrix * gl_Normal);
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	gl_TexCoord[0] = gl_MultiTexCoord0;
}