#include "Vertex.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#define TMVP true

static VertexData *g_VertexBufferData=new VertexData[5*9*4];

Vertex::Vertex()
	:m_uiVertexArrayID(0)
	,m_uiVertexBuffer(0)
{}

Vertex::~Vertex()
{
	glDeleteBuffers(1,&m_uiVertexBuffer);
}

void Vertex::CreateVBO()
{
	glGenBuffers(1,&m_uiVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,m_uiVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,(sizeof(VertexData)*5*9*4),g_VertexBufferData,GL_STATIC_DRAW);
}

GLuint Vertex::GetVertexBuffer()
{
	return m_uiVertexBuffer;
}

void Vertex::SetVertexData()
{
	typedef struct
	{
		GLfloat x;
		GLfloat y;
	}VertexCenter;
	VertexCenter *CurrentCenter=new VertexCenter[45];

	GLfloat fHeight[6]={0};
	fHeight[1]=(GLfloat)15/(GLfloat)75;
	fHeight[2]=(GLfloat)18/(GLfloat)75;
	fHeight[3]=(GLfloat)17/(GLfloat)75;
	fHeight[4]=(GLfloat)16/(GLfloat)75;
	fHeight[5]=(GLfloat)9/(GLfloat)75;

	GLfloat fHeightT[6]={0};
	fHeightT[1]=(GLfloat)15/(GLfloat)75;
	fHeightT[2]=fHeightT[1]+(GLfloat)18/(GLfloat)75;
	fHeightT[3]=fHeightT[2]+(GLfloat)17/(GLfloat)75;
	fHeightT[4]=fHeightT[3]+(GLfloat)16/(GLfloat)75;
	fHeightT[5]=1.0f;

	GLfloat fWidthFirst[10]={0};
	fWidthFirst[1]=(GLfloat)92/(GLfloat)606;
	fWidthFirst[2]=(GLfloat)33/(GLfloat)606;
	fWidthFirst[3]=(GLfloat)71/(GLfloat)606;
	fWidthFirst[4]=(GLfloat)70/(GLfloat)606;
	fWidthFirst[5]=(GLfloat)47/(GLfloat)606;
	fWidthFirst[6]=(GLfloat)94/(GLfloat)606;
	fWidthFirst[7]=(GLfloat)60/(GLfloat)606;
	fWidthFirst[8]=(GLfloat)80/(GLfloat)606;
	fWidthFirst[9]=(GLfloat)59/(GLfloat)606;

	GLfloat fWidthFirstT[10]={0};
	fWidthFirstT[1]=(GLfloat)92/(GLfloat)606;
	fWidthFirstT[2]=fWidthFirstT[1]+(GLfloat)33/(GLfloat)606;
	fWidthFirstT[3]=fWidthFirstT[2]+(GLfloat)71/(GLfloat)606;
	fWidthFirstT[4]=fWidthFirstT[3]+(GLfloat)70/(GLfloat)606;
	fWidthFirstT[5]=fWidthFirstT[4]+(GLfloat)47/(GLfloat)606;
	fWidthFirstT[6]=fWidthFirstT[5]+(GLfloat)94/(GLfloat)606;
	fWidthFirstT[7]=fWidthFirstT[6]+(GLfloat)60/(GLfloat)606;
	fWidthFirstT[8]=fWidthFirstT[7]+(GLfloat)80/(GLfloat)606;
	fWidthFirstT[9]=1;

	GLfloat fWidthOther[10]={0};
	fWidthOther[1]=(GLfloat)55/(GLfloat)606;
	fWidthOther[2]=(GLfloat)70/(GLfloat)606;
	fWidthOther[3]=(GLfloat)70/(GLfloat)606;
	fWidthOther[4]=(GLfloat)71/(GLfloat)606;
	fWidthOther[5]=(GLfloat)47/(GLfloat)606;
	fWidthOther[6]=(GLfloat)94/(GLfloat)606;
	fWidthOther[7]=(GLfloat)60/(GLfloat)606;
	fWidthOther[8]=(GLfloat)80/(GLfloat)606;
	fWidthOther[9]=(GLfloat)59/(GLfloat)606;;

	GLfloat fWidthOtherT[10]={0};
	fWidthOtherT[1]=(GLfloat)55/(GLfloat)606;
	fWidthOtherT[2]=fWidthOtherT[1]+(GLfloat)70/(GLfloat)606;
	fWidthOtherT[3]=fWidthOtherT[2]+(GLfloat)70/(GLfloat)606;
	fWidthOtherT[4]=fWidthOtherT[3]+(GLfloat)71/(GLfloat)606;
	fWidthOtherT[5]=fWidthOtherT[4]+(GLfloat)47/(GLfloat)606;
	fWidthOtherT[6]=fWidthOtherT[5]+(GLfloat)94/(GLfloat)606;
	fWidthOtherT[7]=fWidthOtherT[6]+(GLfloat)60/(GLfloat)606;
	fWidthOtherT[8]=fWidthOtherT[7]+(GLfloat)80/(GLfloat)606;
	fWidthOtherT[9]=1;

	//===//calculate the center dot of every block
	for (int iRow=0;iRow<5;++iRow)
	{
		if (iRow<4)
		{
			for (int iNum=0;iNum<9;++iNum)
			{
				CurrentCenter[iRow*9+iNum].x=fWidthOther[iNum+1]/(GLfloat)2.0+fWidthOtherT[iNum];
				CurrentCenter[iRow*9+iNum].y=fHeight[iRow+1]/(GLfloat)2.0+fHeightT[iRow];
			}
		}
		else
		{
			for (int iNum=0;iNum<9;++iNum)
			{
				CurrentCenter[iRow*9+iNum].x=fWidthFirst[iNum+1]/(GLfloat)2.0+fWidthFirstT[iNum];
				CurrentCenter[iRow*9+iNum].y=fHeight[iRow+1]/(GLfloat)2.0+fHeightT[iRow];
			}
		}
	}

#ifdef TMVP
	//===//Set the project matrix(Destination:to set )
	glm::mat4 ProMatrix=glm::ortho(0.0f,1.0f,0.0f,1.0f,-1.0f,1.0f);
	glm::mat4 Model=glm::mat4(1.0f);
	glm::mat4 TraMatrix=glm::translate(Model,glm::vec3(0.3f,0.3f,0.0f));
	glm::mat4 ScaMatrix=glm::scale(Model,glm::vec3(0.5f,0.5f,0.0f));
	Model=TraMatrix*ScaMatrix;
	glm::mat4 MVP=ProMatrix*Model;

	glm::vec4 vNewCoor;
#endif
	//===//calculate the vertex coordinate of every block
	for (GLint iRow=0;iRow<5;++iRow)
	{
		for (GLint iNum=0;iNum<9;++iNum)
		{
			for (GLint iElement=0;iElement<4;++iElement)
			{
				if (iRow<4 )
				{
				
					if (iElement==0)
					{
						
#ifdef TMVP
						vNewCoor=MVP*glm::vec4(CurrentCenter[iRow*9+iNum].x-fWidthOther[iNum+1]/(GLfloat)2.0,CurrentCenter[iRow*9+iNum].y-fHeight[iRow+1]/(GLfloat)2.0,0.0,1);
#else
						g_VertexBufferData[iRow*36+iNum*4+iElement].x=CurrentCenter[iRow*9+iNum].x-fWidthOther[iNum+1]/(GLfloat)2.0;
						g_VertexBufferData[iRow*36+iNum*4+iElement].y=CurrentCenter[iRow*9+iNum].y-fHeight[iRow+1]/(GLfloat)2.0;
						g_VertexBufferData[iRow*36+iNum*4+iElement].z=0.0;
#endif
						g_VertexBufferData[iRow*36+iNum*4+iElement].u=CurrentCenter[iRow*9+iNum].x-fWidthOther[iNum+1]/(GLfloat)2.0;
						g_VertexBufferData[iRow*36+iNum*4+iElement].v=(GLfloat)1.0-(CurrentCenter[iRow*9+iNum].y-fHeight[iRow+1]/(GLfloat)2.0);

					}
					else if (iElement==1)
					{
	
#ifdef TMVP
						vNewCoor=MVP*glm::vec4(CurrentCenter[iRow*9+iNum].x+fWidthOther[iNum+1]/(GLfloat)2.0,CurrentCenter[iRow*9+iNum].y-fHeight[iRow+1]/(GLfloat)2.0,0.0,1);

#else
						g_VertexBufferData[iRow*36+iNum*4+iElement].x=CurrentCenter[iRow*9+iNum].x+fWidthOther[iNum+1]/(GLfloat)2.0;
						g_VertexBufferData[iRow*36+iNum*4+iElement].y=CurrentCenter[iRow*9+iNum].y-fHeight[iRow+1]/(GLfloat)2.0;
						g_VertexBufferData[iRow*36+iNum*4+iElement].z=0.0;
#endif
						
						g_VertexBufferData[iRow*36+iNum*4+iElement].u=CurrentCenter[iRow*9+iNum].x+fWidthOther[iNum+1]/(GLfloat)2.0;
						g_VertexBufferData[iRow*36+iNum*4+iElement].v=(GLfloat)1.0-(CurrentCenter[iRow*9+iNum].y-fHeight[iRow+1]/(GLfloat)2.0);

					}
					else if (iElement==2)
					{
#ifdef TMVP
						vNewCoor=MVP*glm::vec4(CurrentCenter[iRow*9+iNum].x-fWidthOther[iNum+1]/(GLfloat)2.0,CurrentCenter[iRow*9+iNum].y+fHeight[iRow+1]/(GLfloat)2.0,0.0,1);
#else
						g_VertexBufferData[iRow*36+iNum*4+iElement].x=CurrentCenter[iRow*9+iNum].x-fWidthOther[iNum+1]/(GLfloat)2.0;
						g_VertexBufferData[iRow*36+iNum*4+iElement].y=CurrentCenter[iRow*9+iNum].y+fHeight[iRow+1]/(GLfloat)2.0;
						g_VertexBufferData[iRow*36+iNum*4+iElement].z=0.0;
#endif


						g_VertexBufferData[iRow*36+iNum*4+iElement].u=CurrentCenter[iRow*9+iNum].x-fWidthOther[iNum+1]/(GLfloat)2.0;
						g_VertexBufferData[iRow*36+iNum*4+iElement].v=(GLfloat)1.0-(CurrentCenter[iRow*9+iNum].y+fHeight[iRow+1]/(GLfloat)2.0);

					}
					else if (iElement==3)
					{
#ifdef TMVP
						vNewCoor=MVP*glm::vec4(CurrentCenter[iRow*9+iNum].x+fWidthOther[iNum+1]/(GLfloat)2.0,CurrentCenter[iRow*9+iNum].y+fHeight[iRow+1]/(GLfloat)2.0,0.0,1);
#else
						g_VertexBufferData[iRow*36+iNum*4+iElement].x=CurrentCenter[iRow*9+iNum].x+fWidthOther[iNum+1]/(GLfloat)2.0;
						g_VertexBufferData[iRow*36+iNum*4+iElement].y=CurrentCenter[iRow*9+iNum].y+fHeight[iRow+1]/(GLfloat)2.0;
						g_VertexBufferData[iRow*36+iNum*4+iElement].z=0.0;
#endif
						g_VertexBufferData[iRow*36+iNum*4+iElement].u=CurrentCenter[iRow*9+iNum].x+fWidthOther[iNum+1]/(GLfloat)2.0;
						g_VertexBufferData[iRow*36+iNum*4+iElement].v=(GLfloat)1.0-(CurrentCenter[iRow*9+iNum].y+fHeight[iRow+1]/(GLfloat)2.0);
					}
				}
				else if(iNum!=3 && iNum!=7)
				{
					if (iElement==0)
					{
#ifdef TMVP
						vNewCoor=MVP*glm::vec4(CurrentCenter[iRow*9+iNum].x-fWidthFirst[iNum+1]/(GLfloat)2.0,CurrentCenter[iRow*9+iNum].y-fHeight[iRow+1]/(GLfloat)2.0,0.0,1);
#else
						g_VertexBufferData[iRow*36+iNum*4+iElement].x=CurrentCenter[iRow*9+iNum].x-fWidthFirst[iNum+1]/(GLfloat)2.0;
						g_VertexBufferData[iRow*36+iNum*4+iElement].y=CurrentCenter[iRow*9+iNum].y-fHeight[iRow+1]/(GLfloat)2.0;
						g_VertexBufferData[iRow*36+iNum*4+iElement].z=0.0;
#endif

						g_VertexBufferData[iRow*36+iNum*4+iElement].u=CurrentCenter[iRow*9+iNum].x-fWidthFirst[iNum+1]/(GLfloat)2.0;
						g_VertexBufferData[iRow*36+iNum*4+iElement].v=(GLfloat)1.0-(CurrentCenter[iRow*9+iNum].y-fHeight[iRow+1]/(GLfloat)2.0);

					}
					else if (iElement==1)
					{
#ifdef TMVP
						vNewCoor=MVP*glm::vec4(CurrentCenter[iRow*9+iNum].x+fWidthFirst[iNum+1]/(GLfloat)2.0,CurrentCenter[iRow*9+iNum].y-fHeight[iRow+1]/(GLfloat)2.0,0.0,1);
#else
						g_VertexBufferData[iRow*36+iNum*4+iElement].x=CurrentCenter[iRow*9+iNum].x+fWidthFirst[iNum+1]/(GLfloat)2.0;
						g_VertexBufferData[iRow*36+iNum*4+iElement].y=CurrentCenter[iRow*9+iNum].y-fHeight[iRow+1]/(GLfloat)2.0;
						g_VertexBufferData[iRow*36+iNum*4+iElement].z=0.0;
#endif
						g_VertexBufferData[iRow*36+iNum*4+iElement].u=CurrentCenter[iRow*9+iNum].x+fWidthFirst[iNum+1]/(GLfloat)2.0;
						g_VertexBufferData[iRow*36+iNum*4+iElement].v=(GLfloat)1.0-(CurrentCenter[iRow*9+iNum].y-fHeight[iRow+1]/(GLfloat)2.0);
					}
					else if (iElement==2)
					{
#ifdef TMVP
						vNewCoor=MVP*glm::vec4(CurrentCenter[iRow*9+iNum].x-fWidthFirst[iNum+1]/(GLfloat)2.0,CurrentCenter[iRow*9+iNum].y+fHeight[iRow+1]/(GLfloat)2.0,0.0,1);
#else						
						g_VertexBufferData[iRow*36+iNum*4+iElement].x=CurrentCenter[iRow*9+iNum].x-fWidthFirst[iNum+1]/(GLfloat)2.0;
						g_VertexBufferData[iRow*36+iNum*4+iElement].y=CurrentCenter[iRow*9+iNum].y+fHeight[iRow+1]/(GLfloat)2.0;
						g_VertexBufferData[iRow*36+iNum*4+iElement].z=0.0;
#endif
						g_VertexBufferData[iRow*36+iNum*4+iElement].u=CurrentCenter[iRow*9+iNum].x-fWidthFirst[iNum+1]/(GLfloat)2.0;
						g_VertexBufferData[iRow*36+iNum*4+iElement].v=(GLfloat)1.0-(CurrentCenter[iRow*9+iNum].y+fHeight[iRow+1]/(GLfloat)2.0);

					}
					else if (iElement==3)
					{
#ifdef TMVP
						vNewCoor=MVP*glm::vec4(CurrentCenter[iRow*9+iNum].x+fWidthFirst[iNum+1]/(GLfloat)2.0,CurrentCenter[iRow*9+iNum].y+fHeight[iRow+1]/(GLfloat)2.0,0.0,1);
#else						
						g_VertexBufferData[iRow*36+iNum*4+iElement].x=CurrentCenter[iRow*9+iNum].x+fWidthFirst[iNum+1]/(GLfloat)2.0;
						g_VertexBufferData[iRow*36+iNum*4+iElement].y=CurrentCenter[iRow*9+iNum].y+fHeight[iRow+1]/(GLfloat)2.0;
						g_VertexBufferData[iRow*36+iNum*4+iElement].z=0.0;
#endif
						g_VertexBufferData[iRow*36+iNum*4+iElement].u=CurrentCenter[iRow*9+iNum].x+fWidthFirst[iNum+1]/(GLfloat)2.0;
						g_VertexBufferData[iRow*36+iNum*4+iElement].v=(GLfloat)1.0-(CurrentCenter[iRow*9+iNum].y+fHeight[iRow+1]/(GLfloat)2.0);

					}
				}
#ifdef TMVP
				g_VertexBufferData[iRow*36+iNum*4+iElement].x=vNewCoor.x;
				g_VertexBufferData[iRow*36+iNum*4+iElement].y=vNewCoor.y;
				g_VertexBufferData[iRow*36+iNum*4+iElement].z=0.0;
#endif
			}
		}
	}
}
