#version 150 compatibility

uniform float color;
uniform int seg_num;
flat in int line;

void main(){
	float line_num = line;
    float tmp = line_num / (seg_num-1);
	gl_FragColor = vec4(0.3f * tmp , 0.3f * tmp , 0.7f * tmp , 1.0f - 1.0f*tmp);
	//gl_FragColor = vec4(0.3f * color , 0.3f * color , 0.7f * color , 1.0f - 1.0f*color);
	//gl_FragColor = vec4(0.3f , 0.3f , 0.7f , 1.0f);
}
