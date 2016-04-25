#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
//===============================================================================//
//==Texture class used to load texture data
class Texture
{
public:
	Texture();
	~Texture();
	void LoadTex(const char * texture_file_path);
	GLuint GetTextureID();
	GLint GetTexHeight();
	GLint GetTexWidth();
private:
	GLuint m_iTexture;
	GLint m_iTexWidth;
	GLint m_iTexHeight;
};