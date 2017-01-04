#include "mesh.h"

class scene{
public:
	float scale[100][3];
	float angle[100];
	float rotate[100][3];
	float transfer[100][3];
	int counter;
	mesh *object[100];

	scene(const char* scene_file);
};