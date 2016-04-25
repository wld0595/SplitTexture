#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

//Vertex data struct
typedef struct 
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat u;
	GLfloat v;
}VertexData;

class Vertex
{
public:
	Vertex();
	~Vertex();
	void CreateVBO();
	GLuint GetVertexBuffer();
	void SetVertexData();
	
private:
	GLuint m_uiVertexArrayID;
	GLuint m_uiVertexBuffer;
};