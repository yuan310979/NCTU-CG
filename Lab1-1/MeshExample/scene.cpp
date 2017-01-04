#include "scene.h"
#include "mesh.h"

scene::scene(const char* scene_file){
	FILE *fptr;
	errno_t err;
	err = fopen_s(&fptr, scene_file, "r");
	char token[100];
	char name[100][100];
	counter = 0;

	if (err != 0){
		perror("Error opening file");
	}

	while (!feof(fptr)){
		token[0] = NULL;
		fscanf(fptr, "%s", token); //model
		if (!strcmp(token, "model")){
			fscanf(fptr, "%s", name[counter]); //obj_name
			fscanf(fptr, "%f %f %f", &scale[counter][0], &scale[counter][1], &scale[counter][2]);
			fscanf(fptr, "%f", &angle[counter]);
			fscanf(fptr, "%f %f %f", &rotate[counter][0], &rotate[counter][1], &rotate[counter][2]);
			fscanf(fptr, "%f %f %f", &transfer[counter][0], &transfer[counter][1], &transfer[counter][2]);
			counter++;
		}
	}

	for (int i = 0; i < counter; i++){
		object[i] = new mesh(name[i]);
	}
}