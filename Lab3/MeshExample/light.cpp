#include "light.h"
#include <stdio.h>
#include <string.h>

lightning::lightning(const char* light_file){
	FILE *Light;
	errno_t err;
	err = fopen_s(&Light, light_file, "r");
	char token[100];
	counter = 0;

	if (err != 0){
		perror("Error opening file");
	}

	while (!feof(Light)){
		token[0] = NULL;
		fscanf(Light, "%s", token);

		if (!strcmp(token, "light")){
			fscanf(Light, "%f %f %f", &x[counter], &y[counter], &z[counter]);
			fscanf(Light, "%f %f %f", &ambient[counter][0], &ambient[counter][1], &ambient[counter][2]);
			fscanf(Light, "%f %f %f", &diffuse[counter][0], &diffuse[counter][1], &diffuse[counter][2]);
			fscanf(Light, "%f %f %f", &specular[counter][0], &specular[counter][1], &specular[counter][2]);
			counter++;
		}
		else if (!strcmp(token, "ambient")){
			fscanf(Light, "%f %f %f", &environment_ambient[0], &environment_ambient[1], &environment_ambient[2]);
		}
	}
}