#ifndef AFX_TEXTURE_H_
#define AFX_TEXTURE_H_

#include <mesh.h>

class texture
{
public:
	int iWidth;
	int iHeight;
	unsigned int texObject[1]; // the generated texture object

	texture();
	~texture();
	void LoadTexture(char*);
	void LoadEnvTexture(char*, char*, char*, char*, char*, char*);
	void Bind();
	void GenerateMipMap();
};

#endif