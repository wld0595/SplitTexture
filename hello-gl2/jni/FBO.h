#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

//===============================================================================//
//==Texture class used to load texture data
class Fbo
{
public:
	Fbo();
	~Fbo();
	void GenFBO();
	GLuint GetFrameBufferID();
	GLuint GetFrameTextureID();
	void SetWidth(GLint);
	void SetHeight(GLint);
private:
	GLuint m_iFrameBuffer;
	GLuint m_iRenderTexture;
	GLint m_iTexWidth;
	GLint m_iTexHeight;
};