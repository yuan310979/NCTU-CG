#include "scene.h"
#include "mesh.h"

scene::scene(const char* scene_file){
	FILE *fptr;
	errno_t err;
	err = fopen_s(&fptr, scene_file, "r");
	char token[100];
	char name[100][100];
	counter = 0;
	tex_counter = 0;
	int type;

	if (err != 0){
		perror("Error opening file");
	}

	while (!feof(fptr)){
		token[0] = NULL;
		fscanf(fptr, "%s", token); //model
		if (!strcmp(token, "no-texture")){
			type = 0;
		}
		else if (!strcmp(token, "single-texture")){
			type = 1;
			tex_type[tex_counter] = 1;
			fscanf(fptr, "%s", tex_name[tex_counter][0]);
			tex_counter++;
		}
		else if (!strcmp(token, "multi-texture")){
			type = 2;
			for (int i = 0; i < 2; i++){
				fscanf(fptr, "%s", tex_name[tex_counter][0]);
				tex_type[tex_counter] = 2;
				tex_counter++;
			}
		}
		else if (!strcmp(token, "cube-map")){
			type = 3;
			tex_type[tex_counter] = 3;
			fscanf(fptr, "%s%s%s%s%s%s", tex_name[tex_counter][0], tex_name[tex_counter][1], tex_name[tex_counter][2], tex_name[tex_counter][3], tex_name[tex_counter][4], tex_name[tex_counter][5]);
			tex_counter++;
		}
		else if (!strcmp(token, "model")){
			fscanf(fptr, "%s", name[counter]); //obj_name
			fscanf(fptr, "%f %f %f", &scale[counter][0], &scale[counter][1], &scale[counter][2]);
			fscanf(fptr, "%f", &angle[counter]);
			fscanf(fptr, "%f %f %f", &rotate[counter][0], &rotate[counter][1], &rotate[counter][2]);
			fscanf(fptr, "%f %f %f", &transfer[counter][0], &transfer[counter][1], &transfer[counter][2]);

			obj_tex_type[counter] = type;
			obj_tex_index[counter] = tex_counter - 1; //last index of the texture (ex: 3 4 => 4)

			counter++;
		}
	}

	for (int i = 0; i < counter; i++){
		object[i] = new mesh(name[i]);
		printf("%s: %d %d\n", name[i], obj_tex_type[i], obj_tex_index[i]);
	}
}