#include "FBO.h"
#include <stdio.h>

Fbo::Fbo()
	:m_iFrameBuffer(0)
	,m_iRenderTexture(0)
{}

Fbo::~Fbo()
{
	glDeleteFramebuffers(1, &m_iFrameBuffer);
	glDeleteTextures(1, &m_iRenderTexture);
}

void Fbo::SetWidth(GLint w)
{
	m_iTexWidth=w;
}

void Fbo::SetHeight(GLint h)
{
	m_iTexHeight=h;
}

void Fbo::GenFBO()
{
	/*GLint iWidth=606,iHeight=75;*/
	glGenTextures(1, &m_iRenderTexture);
	glBindTexture(GL_TEXTURE_2D, m_iRenderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, m_iTexWidth, m_iTexWidth, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	glGenFramebuffers(1,&m_iFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER,m_iFrameBuffer);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, m_iRenderTexture, 0);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return ;
}

GLuint Fbo::GetFrameBufferID()
{
	return m_iFrameBuffer;
}

GLuint Fbo::GetFrameTextureID()
{
	return m_iRenderTexture;
}
