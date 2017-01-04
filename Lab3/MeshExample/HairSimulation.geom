#version 150 compatibility

layout(triangles) in;
layout(line_strip, max_vertices=100) out;

in Vertex{
    vec3 normal;
}vertex[];

flat out int line;

uniform int num;
uniform vec4 gravity;
uniform float length; 

uniform int seg_num;

void main(){
	vec4 nor;
	vec4 pos;
	for(int i = 0; i < gl_in.length(); i++){
		pos = gl_in[i].gl_Position;
		nor = normalize(vec4(vertex[i].normal, 0.0f) + gravity);
		gl_Position = gl_ProjectionMatrix * pos;
		EmitVertex();
		for(int j = 1; j < seg_num; j++){
			line = j;
			gl_Position = gl_ProjectionMatrix * (pos + nor*length);
			EmitVertex();
			pos += nor*length;
			nor =  normalize(nor + gravity);
		}
		EndPrimitive();
	}
}

/*
void main(){
	vec4 nor;
	vec4 pos;
	for(int i = 0; i < gl_in.length(); i++){
		pos = gl_in[i].gl_Position;
		nor = normalize(vec4(vertex[i].normal, 0.0f) + gravity);
		for(int j = 0; j < num; j++){
			pos = pos + nor*length;
			nor += gravity;
			nor = normalize(nor);
		}
		gl_Position = gl_ProjectionMatrix * pos;
		EmitVertex();
		//end point
		gl_Position = gl_ProjectionMatrix * (pos + nor*length);
		EmitVertex();
		EndPrimitive();
	}
}
*/