#include "texture.h"
#include "FreeImage.h"
#include <GL/glew.h>

texture::texture(){
	glGenTextures(1, texObject);
}

texture::~texture(){
	glDeleteTextures(1, texObject);
}

void texture::LoadTexture(char* pFilename){
	// Load texture file to memory
	FreeImage_Initialise();
	FIBITMAP* pImage = FreeImage_Load(FreeImage_GetFileType(pFilename, 0), pFilename);
	FIBITMAP* p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
	iWidth = FreeImage_GetWidth(p32BitsImage);
	iHeight = FreeImage_GetHeight(p32BitsImage);
	// Load from memory to GPU
	Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, iWidth, iHeight,
		0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(p32BitsImage));
	//glGenerateMipmap(GL_TEXTURE_2D);
	
	/*gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, iWidth, iHeight,
		GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(p32BitsImage));*/

	GenerateMipMap();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	/*glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);*/

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // �̨ήĪG

	FreeImage_Unload(p32BitsImage);
	FreeImage_Unload(pImage);
}

void texture::LoadEnvTexture(char* pFilename0, char* pFilename1, char* pFilename2, char* pFilename3, char* pFilename4, char* pFilename5){
	FIBITMAP* pImage;
	FIBITMAP* p32BitsImage;
	glBindTexture(GL_TEXTURE_CUBE_MAP, texObject[0]);
	for (int i = 0; i < 6; i++){
		if (i == 0){
			FreeImage_Initialise();
			pImage = FreeImage_Load(FreeImage_GetFileType(pFilename0, 0), pFilename0);
			p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
			iWidth = FreeImage_GetWidth(p32BitsImage);
			iHeight = FreeImage_GetHeight(p32BitsImage);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA,
				iWidth, iHeight, 0, GL_BGR, GL_UNSIGNED_BYTE,
				(void*)FreeImage_GetBits(pImage));

		}
		else if (i == 1){
			FreeImage_Initialise();
			pImage = FreeImage_Load(FreeImage_GetFileType(pFilename1, 0), pFilename1);
			p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
			iWidth = FreeImage_GetWidth(p32BitsImage);
			iHeight = FreeImage_GetHeight(p32BitsImage);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA,
				iWidth, iHeight, 0, GL_BGR, GL_UNSIGNED_BYTE,
				(void*)FreeImage_GetBits(pImage));
		}
		else if (i == 2){
			FreeImage_Initialise();
			pImage = FreeImage_Load(FreeImage_GetFileType(pFilename2, 0), pFilename2);
			p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
			iWidth = FreeImage_GetWidth(p32BitsImage);
			iHeight = FreeImage_GetHeight(p32BitsImage);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA,
				iWidth, iHeight, 0, GL_BGR, GL_UNSIGNED_BYTE,
				(void*)FreeImage_GetBits(pImage));
		}
		else if (i == 3){
			FreeImage_Initialise();
			pImage = FreeImage_Load(FreeImage_GetFileType(pFilename3, 0), pFilename3);
			p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
			iWidth = FreeImage_GetWidth(p32BitsImage);
			iHeight = FreeImage_GetHeight(p32BitsImage);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA,
				iWidth, iHeight, 0, GL_BGR, GL_UNSIGNED_BYTE,
				(void*)FreeImage_GetBits(pImage));
		}
		else if (i == 4){
			FreeImage_Initialise();
			pImage = FreeImage_Load(FreeImage_GetFileType(pFilename4, 0), pFilename4);
			p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
			iWidth = FreeImage_GetWidth(p32BitsImage);
			iHeight = FreeImage_GetHeight(p32BitsImage);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA,
				iWidth, iHeight, 0, GL_BGR, GL_UNSIGNED_BYTE,
				(void*)FreeImage_GetBits(pImage));
		}
		else if (i == 5){
			FreeImage_Initialise();
			pImage = FreeImage_Load(FreeImage_GetFileType(pFilename5, 0), pFilename5);
			p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
			iWidth = FreeImage_GetWidth(p32BitsImage);
			iHeight = FreeImage_GetHeight(p32BitsImage);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA,
				iWidth, iHeight, 0, GL_BGR, GL_UNSIGNED_BYTE,
				(void*)FreeImage_GetBits(pImage));
		}
	}

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	FreeImage_Unload(p32BitsImage);
	FreeImage_Unload(pImage);
}

void texture::Bind(){
	glBindTexture(GL_TEXTURE_2D, texObject[0]);
}

void texture::GenerateMipMap(){
	glGenerateMipmap(GL_TEXTURE_2D);
}