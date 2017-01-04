#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "view.h"

view::view(const char* view_file){
	FILE *View;
	errno_t err;
	err = fopen_s(&View ,view_file, "r");
	char token[100];

	if (err != 0){
		perror("Error opening file");
	}

	while (!feof(View))
	{
		token[0] = NULL;
		fscanf(View, "%s", token);		// ? token

		if (!strcmp(token, "eye"))
		{
			fscanf(View, "%f %f %f", &eye[0], &eye[1], &eye[2]);
		}

		else if (!strcmp(token, "vat"))
		{
			fscanf(View, "%f %f %f", &vat[0], &vat[1], &vat[2]);
		}

		else if (!strcmp(token, "vup"))
		{
			fscanf(View, "%f %f %f", &vup[0], &vup[1], &vup[2]);
		}

		else if (!strcmp(token, "fovy"))
		{
			fscanf(View, "%f", &fovy);
		}

		else if (!strcmp(token, "dnear"))
		{
			fscanf(View, "%f", &dnear);
		}
		else if (!strcmp(token, "dfar"))
		{
			fscanf(View, "%f", &dfar);
		}
		else if (!strcmp(token, "viewport"))
		{
			fscanf(View, "%f %f %f %f", &viewport[0], &viewport[1], &viewport[2], &viewport[3]);
		}
	}
}