#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Texture.h"

//===============================================================================//
//Texture class

//===Construction function:initialized the member variable
Texture::Texture()
	:m_iTexture(0)
	,m_iTexWidth(0)
	,m_iTexHeight(0)
	{}

//===Destruct function:release the memory
Texture::~Texture()
{
	glDeleteTextures(1,&m_iTexture);
}

//===Load the "iNum" numbers of texture file
void Texture::LoadTex(const char * texture_file_path)
{
	//===Load the texture file 
	GLint iChannel=0;
	FILE* pFile=fopen(texture_file_path,"rb");
	const unsigned char* pImageData=stbi_load_from_file(pFile,&m_iTexWidth,&m_iTexHeight,&iChannel,0);

	//====//check the texture picture 
	if (pImageData==NULL)
	{
		fprintf(stderr,"The %s texture picture is null.Please check.\n",texture_file_path);
		getchar();
		return ;
	}

	//Generate the texture id
	glGenTextures(1,&m_iTexture);
	glBindTexture(GL_TEXTURE_2D,m_iTexture);
	//====//
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	//===//check the texture id
	if (glIsTexture(m_iTexture)==GL_FALSE)
	{
		fprintf(stderr,"Is not currently the name of texture.Or some error occurs.Please check.\n");
		getchar();
		return ;
	}

	//Generate the texture
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,m_iTexWidth,m_iTexHeight,0,GL_RGB,GL_UNSIGNED_BYTE,pImageData);
	//Release the texture picture buffer
	free((void *)pImageData);
	pImageData=NULL;
	//Set the texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
 	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
 	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
}

//===Get the iNum-th texture id
GLuint Texture::GetTextureID()
{
	return m_iTexture;
}

GLint Texture::GetTexWidth()
{
	return m_iTexWidth;
}

GLint Texture::GetTexHeight()
{
	return m_iTexHeight;
}