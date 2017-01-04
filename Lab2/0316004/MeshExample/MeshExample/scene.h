#include "mesh.h"
#include "texture.h"
#include <string>

class scene{
public:
	float scale[100][3];
	float angle[100];
	float rotate[100][3];
	float transfer[100][3];
	int counter;
	mesh *object[100];

	char tex_name[100][6][100]; //最多6個(cube)
	int tex_type[100]; //// 1:Single 2.multi(2個) 3.cube
	int obj_tex_index[100]; 
	int obj_tex_type[100]; // 0:No texture 1:Single 2.multi(2個) 3.cube
	int tex_counter;

	scene(const char* scene_file);
};