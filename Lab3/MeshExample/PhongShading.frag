#version 150 compatibility

// eye cooerdinate
in vec3 vertexEyeSpace;
// normal vector
in vec3 normal;
uniform int light_num;
uniform sampler2D colorTexture;

void main(){
	vec4 Ia, Id, Is;
	vec4 finalFragColor = vec4(0.0f);
	vec3 N = normalize(normal);
	vec3 V = normalize(-vertexEyeSpace);

	for(int i = 0; i < light_num; i++){
		vec3 L = normalize(gl_LightSource[i].position.xyz - vertexEyeSpace);
		vec3 R = normalize(reflect(-L, N));

		 // ambient = Ia * Ka
        Ia = gl_LightSource[i].ambient * gl_FrontMaterial.ambient;

        // diffuse = Id * Kd * (N * L)
        Id = gl_LightSource[i].diffuse * gl_FrontMaterial.diffuse * max(dot(N, L), 0.0f);

        // specular = Is * Ks * (R * V)^ns
        Is = gl_LightSource[i].specular * gl_FrontMaterial.specular * pow(max(dot(R, V), 0.0f), gl_FrontMaterial.shininess);

        finalFragColor += Ia + Id + Is;
	}

	gl_FragColor = vec4(finalFragColor) * texture(colorTexture, gl_TexCoord[0].xy);
}	